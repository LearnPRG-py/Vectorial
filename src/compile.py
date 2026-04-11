import subprocess
import sys
from pathlib import Path

build_dir = Path("build")
subprocess.run(["cmake", "-B", str(build_dir)], check=True)
subprocess.run(["cmake", "--build", str(build_dir)], check=True)

if sys.platform == "win32":
    test_bin = build_dir / "Debug" / "test.exe"
else:
    test_bin = build_dir / "test"

subprocess.run([str(test_bin)], check=True)
