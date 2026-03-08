import webbrowser
import time

URL = "https://www.google.com"

for i in range(200):
    webbrowser.open_new_tab(URL)
    time.sleep(0.2)
