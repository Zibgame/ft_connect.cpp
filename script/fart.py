import os
import sys
import time
import random
import subprocess
import numpy as np

CONFIG = {
    "sounds": [
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/other/sound/fart.mp3",
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/other/sound/fart1.mp3",
        "/sgoinfre/goinfre/Perso/zcadinot/.fcpp/other/sound/fart2.mp3"
    ],
    "volume_percent": "40%",
    "gain": 1.0,
    "clip_min": -32768,
    "clip_max": 32767,
    "sleep_step": 0.1
}

DEPS = os.path.join(os.path.dirname(__file__), ".deps")

def silence():
    devnull = os.open(os.devnull, os.O_WRONLY)
    os.dup2(devnull, 1)
    os.dup2(devnull, 2)

def enable_sound():
    subprocess.run(
        "pactl set-sink-mute @DEFAULT_SINK@ 0",
        shell=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )
    subprocess.run(
        f"pactl set-sink-volume @DEFAULT_SINK@ {CONFIG['volume_percent']}",
        shell=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

def disable_sound():
    subprocess.run(
        "pactl set-sink-volume @DEFAULT_SINK@ 0%",
        shell=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )

def ensure_deps():
    if DEPS not in sys.path:
        sys.path.insert(0, DEPS)
    try:
        import pygame
        import numpy
    except ImportError:
        subprocess.check_call(
            [sys.executable, "-m", "pip", "install", "--target", DEPS, "pygame", "numpy"],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )

silence()
ensure_deps()
sys.path.insert(0, DEPS)

import pygame
import numpy as np
import pygame.sndarray

enable_sound()

sound_path = random.choice(CONFIG["sounds"])

pid = os.fork()

if pid == 0:
    silence()

    pygame.init()
    pygame.mixer.init()

    sound = pygame.mixer.Sound(sound_path)

    arr = pygame.sndarray.array(sound)

    arr = arr * CONFIG["gain"]

    arr = np.clip(
        arr,
        CONFIG["clip_min"],
        CONFIG["clip_max"]
    ).astype(np.int16)

    distorted = pygame.sndarray.make_sound(arr)

    distorted.play()

    while pygame.mixer.get_busy():
        time.sleep(CONFIG["sleep_step"])

    disable_sound()

    os._exit(0)