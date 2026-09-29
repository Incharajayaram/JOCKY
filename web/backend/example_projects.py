EXAMPLE_PROJECTS = {
    "hello_world_windows": {
        "platform": "windows",
        "name": "Hello World (Windows)",
        "description": "Simple Windows payload that prints a message",
        "source": '''fn main() {
    println!("Hello from JOCKY!");
}''',
    },
    "hello_world_linux": {
        "platform": "linux",
        "name": "Hello World (Linux)",
        "description": "Simple Linux payload that prints a message",
        "source": '''fn main() {
    println!("Hello from JOCKY on Linux!");
}''',
    },
    "advanced_windows": {
        "platform": "windows",
        "name": "Advanced Techniques (Windows)",
        "description": "Demonstrates anti-forensics and evasion",
        "source": '''// Advanced Windows techniques
fn main() {
    // This example would use the runtime APIs
    // For anti-forensics, registry operations, etc.
    println!("Advanced payload");
}''',
    },
    "advanced_linux": {
        "platform": "linux",
        "name": "Advanced Techniques (Linux)",
        "description": "Demonstrates Linux-specific capabilities",
        "source": '''// Advanced Linux techniques
fn main() {
    // This example would use Linux runtime APIs
    println!("Advanced Linux payload");
}''',
    },
}


def get_example(example_id: str) -> dict:
    return EXAMPLE_PROJECTS.get(example_id)


def list_examples(platform: str = None) -> list[dict]:
    examples = list(EXAMPLE_PROJECTS.values())
    if platform:
        examples = [e for e in examples if e["platform"] == platform]
    return examples
