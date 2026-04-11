import sys
import subprocess

def install_deps():
    if sys.platform == "darwin":
        subprocess.run(["brew", "install", "cmake", "gcc"], check=True)
    elif sys.platform == "win32":
        subprocess.run(["choco", "install", "cmake", "mingw", "-y"], check=True)
    elif sys.platform == "linux":
        subprocess.run(["sudo", "apt", "install", "-y", "cmake", "g++"], check=True)
    subprocess.run([sys.executable, "-m", "pip", "install", "black==22.3.0"], check=True)

install_deps()
