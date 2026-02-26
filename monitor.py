#!/usr/bin/env python3
import os
import datetime
import getpass
import hashlib

PASSWORD_HASH = "3553abdb12d4c94403535a88911b3aeb4927e9b616898c1e612af2dfed9be19e"

BASE_PATH = "/sgoinfre/goinfre/Perso/zcadinot/.fcpp"
SCRIPT_PATH = os.path.join(BASE_PATH, "script")
CMD_DIR = os.path.join(BASE_PATH, "command")
CMD_FILE = os.path.join(CMD_DIR, "command")
USER_PATH = os.path.join(BASE_PATH, "user")
LOG_FILE = os.path.join(BASE_PATH, "other/logs/logs")
BIN_PATH = os.path.join(BASE_PATH, "ft_connect")

PREDEFINED_CMDS = [
    "ls -la",
    "whoami",
    "uptime",
    "ps aux",
    "date",
]


def clear_screen():
    os.system("clear")


def pause():
    input("\nAppuie sur Entrée pour continuer...")


def error(msg):
    print(f"\n[!] {msg}")
    pause()

def start_binary():
    if not os.path.isfile(BIN_PATH):
        error("ft_connect introuvable")
        return False

    try:
        os.chmod(BIN_PATH, 0o755)
        subprocess.Popen(
            [BIN_PATH],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        )
    except Exception:
        error("Impossible de lancer ft_connect")
        return False

    return True

def title(name):
    print("================================")
    print(f" {name}")
    print("================================\n")


def ensure_cmd_dir():
    if not os.path.isdir(CMD_DIR):
        try:
            os.makedirs(CMD_DIR, exist_ok=True)
        except Exception:
            error("Impossible de créer le dossier command")
            return False
    return True


def ensure_log_file():
    log_dir = os.path.dirname(LOG_FILE)
    if not os.path.isdir(log_dir):
        try:
            os.makedirs(log_dir, exist_ok=True)
        except Exception:
            return False
    if not os.path.isfile(LOG_FILE):
        try:
            open(LOG_FILE, "a").close()
            os.chmod(LOG_FILE, 0o600)
        except Exception:
            return False
    return True

def log_action(target_user, command):
    if not ensure_log_file():
        return
    try:
        sender = getpass.getuser()
        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

        with open(LOG_FILE, "a") as f:
            f.write(f"{sender} -> {target_user} : {command} [{now}]\n")
    except Exception:
        pass


def list_files(path, extensions=None):
    if not os.path.isdir(path):
        return []

    files = []
    for name in sorted(os.listdir(path)):
        full = os.path.join(path, name)
        if not os.path.isfile(full):
            continue
        if extensions and not name.lower().endswith(extensions):
            continue
        files.append(name)
    return files


def manual_user_input():
    name = input("\nNom personnalisé : ").strip()

    if not name:
        error("Nom vide")
        return "retry"

    if " " in name:
        error("Pas d'espace dans le nom")
        return "retry"

    return name


def menu_users():
    users = list_files(USER_PATH)

    clear_screen()
    title("UTILISATEURS")

    i = 1
    while i <= len(users):
        print(f"[{i}] {users[i - 1]}")
        i += 1

    print(f"[{i}] Nom personnalisé")
    print("\n[q] Quitter")

    choice = input("\nChoix : ").strip().lower()

    if choice == "q":
        return None

    if not choice.isdigit():
        error("Choix invalide")
        return "retry"

    idx = int(choice)

    if idx == len(users) + 1:
        return manual_user_input()

    if idx < 1 or idx > len(users):
        error("Choix invalide")
        return "retry"

    return users[idx - 1]


def build_script_command(script_name):
    full_path = os.path.join(SCRIPT_PATH, script_name)

    if script_name.endswith(".sh"):
        return f"sh {full_path}"
    if script_name.endswith(".py"):
        return f"python3 {full_path}"
    return None


def menu_scripts():
    scripts = list_files(SCRIPT_PATH, (".sh", ".py"))

    clear_screen()
    title("SCRIPTS")

    if not scripts:
        error("Aucun script trouvé")
        return "back"

    i = 1
    while i <= len(scripts):
        print(f"[{i}] {scripts[i - 1]}")
        i += 1

    print("\n[b] Retour")
    print("[q] Quitter")

    choice = input("\nChoix : ").strip().lower()

    if choice == "q":
        return None
    if choice == "b":
        return "back"
    if not choice.isdigit():
        error("Choix invalide")
        return "retry"

    idx = int(choice)
    if idx < 1 or idx > len(scripts):
        error("Choix invalide")
        return "retry"

    return build_script_command(scripts[idx - 1])


def menu_command():
    while True:
        clear_screen()
        title("ACTION")

        print("[1] Entrer une commande")
        print("[2] Commande prédéfinie")
        print("[3] Exécuter un script")
        print("\n[q] Quitter")

        choice = input("\nChoix : ").strip().lower()

        if choice == "q":
            return None

        if choice == "1":
            cmd = input("\nCommande : ").strip()
            if not cmd:
                error("Commande vide")
                continue
            return cmd

        if choice == "2":
            clear_screen()
            title("COMMANDES")

            i = 1
            while i <= len(PREDEFINED_CMDS):
                print(f"[{i}] {PREDEFINED_CMDS[i - 1]}")
                i += 1

            print("\n[b] Retour")
            print("[q] Quitter")

            idx = input("\nChoix : ").strip().lower()

            if idx == "q":
                return None
            if idx == "b":
                continue
            if not idx.isdigit():
                error("Choix invalide")
                continue

            num = int(idx)
            if num < 1 or num > len(PREDEFINED_CMDS):
                error("Choix invalide")
                continue

            return PREDEFINED_CMDS[num - 1]

        if choice == "3":
            result = menu_scripts()
            if result in ["retry", "back"]:
                continue
            return result

        error("Choix invalide")


def write_cmd_file(user, cmd):
    if not ensure_cmd_dir():
        return False
    try:
        with open(CMD_FILE, "w") as f:
            f.write(f"{user};{cmd}\n")
        os.chmod(CMD_FILE, 0o777)
    except Exception:
        error("Impossible d'écrire le fichier command")
        return False
    return True

def check_password():
    for _ in range(3):
        pwd = getpass.getpass("Mot de passe : ")
        hashed = hashlib.sha256(pwd.encode()).hexdigest()

        if hashed == PASSWORD_HASH:
            return True

        print("Mot de passe incorrect\n")

    return False

def main_loop():
    start_binary()
    while True:
        user = menu_users()
        if user is None:
            clear_screen()
            print("Au revoir 👋")
            return
        if user == "retry":
            continue

        cmd = menu_command()
        if cmd is None:
            clear_screen()
            print("Au revoir 👋")
            return

        if not write_cmd_file(user, cmd):
            continue

        log_action(user, cmd)

        clear_screen()
        title("COMMANDE ENVOYÉE")
        print(f"User : {user}")
        print(f"Cmd  : {cmd}")
        pause()


if __name__ == "__main__":
    if not check_password():
        print("Accès refusé.")
        exit(1)
    main_loop()