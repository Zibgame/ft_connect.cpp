import os
import sys
import time
import subprocess

SND = "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/other/sound/wow.mp3"
DEPS = os.path.join(os.path.dirname(__file__), ".deps")

def enable_sound():
    subprocess.run("pactl set-sink-mute @DEFAULT_SINK@ 0", shell=True)
    subprocess.run("pactl set-sink-volume @DEFAULT_SINK@ 40%", shell=True)

def ensure_pygame():
    if DEPS not in sys.path:
        sys.path.insert(0, DEPS)
    try:
        import pygame
    except ImportError:
        subprocess.check_call(
            [sys.executable, "-m", "pip", "install", "--target", DEPS, "pygame"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )


ensure_pygame()
sys.path.insert(0, DEPS)
import pygame
enable_sound()

pid = os.fork()

if pid == 0:
    pygame.init()
    pygame.mixer.init()
    pygame.mixer.music.load(SND)
    pygame.mixer.music.set_volume(0.9)
    pygame.mixer.music.play()

    while pygame.mixer.music.get_busy():
        time.sleep(0.8)

    os._exit(0)