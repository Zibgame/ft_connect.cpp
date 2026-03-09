import subprocess

terminals = [
    "gnome-terminal",
    "konsole",
    "xterm",
    "lxterminal",
    "xfce4-terminal",
    "mate-terminal",
    "tilix",
    "alacritty"
    "kitty"
]

for term in terminals:
    subprocess.run(
        ["pkill", "-f", term],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )