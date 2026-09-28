class Jocky < Formula
  desc "JOCKY - Obfuscating C++ compiler with advanced evasion techniques"
  homepage "https://github.com/devalgupta/JOCKY"
  url "https://github.com/devalgupta/JOCKY/archive/v#{version}.tar.gz"
  version "0.1.0"
  sha256 "0d1c95f68cd12d75a3dd3fa5a5b7e4d3a2b8c9f1e2d3c4b5a6f7e8d9c0a1b2c"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "llvm@14" => :build
  depends_on "python@3.10"
  depends_on "gcc"

  def install
    system "cmake", "-B", "build", "-S", ".",
           "-DCMAKE_INSTALL_PREFIX=#{prefix}",
           "-DCMAKE_BUILD_TYPE=Release",
           "-DLLVM_ROOT=#{Formula["llvm@14"].opt_prefix}"

    system "cmake", "--build", "build", "--", "-j#{ENV.make_jobs}"
    system "cmake", "--install", "build"

    bin.install "build/compiler/jockyc"

    # Install Python modules
    libexec.install Dir["src/jocky/*"]

    # Create wrapper script that sets PYTHONPATH
    (bin/"jocky").write_env_script libexec/"cli.py",
      :PYTHONPATH => libexec
  end

  test do
    system "#{bin}/jocky", "--version"
  end
end
