import subprocess
from pathlib import Path

files = list(Path("src").rglob("*.cpp")) + list(Path("src").rglob("*.h"))
for f in files:
    subprocess.run(["clang-format", "-i", str(f)])

py_files = list(Path("src").rglob("*.py"))
for f in py_files:
    subprocess.run(["black", str(f)])
