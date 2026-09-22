import asyncio
import os
import requests
from playwright.async_api import async_playwright

CONCURRENCY = 15 # 15 simultaneous browser tabs to speed things up immensely

def download_file_sync(url, save_path):
    try:
        response = requests.get(url, stream=True, timeout=60)
        if response.status_code == 200:
            with open(save_path, 'wb') as f:
                for chunk in response.iter_content(chunk_size=8192):
                    f.write(chunk)
            return True
    except Exception as e:
        pass
    return False

async def download_file(url, save_path):
    return await asyncio.to_thread(download_file_sync, url, save_path)

async def process_task(task, context, semaphore):
    async with semaphore:
        filename = task['filename']
        sha256 = task['sha256']
        url = task['url']
        save_path = os.path.join('drivers_out', filename)
        
        # Don't re-download if we already got it
        if os.path.exists(save_path):
            return
            
        page = await context.new_page()
        try:
            # Go to Winbindex search page
            await page.goto(url, wait_until='domcontentloaded', timeout=30000)
            
            # Look for the download button pointing to msdl.microsoft.com
            # We wait up to 10 seconds for it to appear as JS loads the data
            download_link = None
            try:
                locator = page.locator('a[href*="msdl.microsoft.com/download/symbols"]')
                await locator.first.wait_for(timeout=10000)
                download_link = await locator.first.get_attribute('href')
            except Exception:
                # Button didn't appear, likely not indexed
                pass
                
            if download_link:
                print(f" [+] Found {filename} on Winbindex! Downloading...")
                success = await download_file(download_link, save_path)
                if success:
                    print(f"   => [SUCCESS] Saved {filename}")
                else:
                    print(f"   => [FAILED] Download failed for {filename}")
            else:
                print(f" [-] Not available on Winbindex: {filename}")
                
        except Exception as e:
            print(f" [!] Error processing {filename}")
        finally:
            await page.close()

async def main():
    if not os.path.exists('winbindex_missing_links.txt'):
        print("Could not find winbindex_missing_links.txt")
        return
        
    tasks = []
    current_task = {}
    with open('winbindex_missing_links.txt', 'r') as f:
        for line in f:
            line = line.strip()
            if line.startswith('Driver:'):
                current_task['filename'] = line.split('Driver:', 1)[1].strip()
            elif line.startswith('SHA256:'):
                current_task['sha256'] = line.split('SHA256:', 1)[1].strip()
            elif line.startswith('Link:'):
                current_task['url'] = line.split('Link:', 1)[1].strip()
                tasks.append(current_task)
                current_task = {}
                
    print(f"Loaded {len(tasks)} missing drivers to query on Winbindex.")
    print(f"Running with a concurrency of {CONCURRENCY} browsers. This may take some time...")
    
    os.makedirs('drivers_out', exist_ok=True)
    
    semaphore = asyncio.Semaphore(CONCURRENCY)
    
    async with async_playwright() as p:
        # Use Chromium in headless mode
        browser = await p.chromium.launch(headless=True)
        # Block unnecessary resources to speed up page loads significantly
        context = await browser.new_context()
        
        async def route_interceptor(route):
            if route.request.resource_type in ["image", "media", "font", "stylesheet"]:
                await route.abort()
            else:
                await route.continue_()
                
        # Apply the interceptor to all pages to save bandwidth/time
        # We can't apply it globally to context easily in async without page.route("**/*", route_interceptor)
        # So we'll skip interceptor for simplicity, DOMContentLoaded is fast enough.

        coroutines = [process_task(t, context, semaphore) for t in tasks]
        
        # Run them all
        await asyncio.gather(*coroutines)
            
        await browser.close()
        
    print("\n[*] Winbindex scraping complete!")

if __name__ == "__main__":
    asyncio.run(main())
