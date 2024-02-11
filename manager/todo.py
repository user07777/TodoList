import json
import sys
import argparse
import winreg
import os
class manager:
    def __init__(self, file="data.json"):
        self.file = file

    def create_initial_object(self):
        initial_data = {"sub": "", "txt": []}
        with open(self.file, "w", encoding="utf-8") as f:
            json.dump(initial_data, f, indent=4)

    def add_object(self):
        try:
            with open(self.file, "r", encoding="utf-8") as f:
                data = json.load(f)
                if not isinstance(data, list):
                    data = []
        except FileNotFoundError:
            print("json não encontrado")
            return

        print("digite o subtitulo:")
        sub = input()

        obj = {"sub": sub, "txt": []}
        print("Digite tarefas para o tópico {} ('exit' para acabar):".format(sub))
        while True:
            t = input()
            if t == "exit":
                break
            obj["txt"].append(t)

        data.append(obj)

        with open(self.file, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=4)

        print("sucesso")

    def complete(self, taskN):
        mark = "✓"
        try:
            with open(self.file, "r", encoding="utf-8") as f:
                data = json.load(f)
        except FileNotFoundError:
            print("json não encontrado")
            return

        for entry in data:
            for i, task in enumerate(entry["txt"]):
                if task.startswith(taskN):
                    entry["txt"][i] = f"{mark} {task}"

        with open(self.file, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=4, ensure_ascii=False)

        print(f"'{taskN}' completada!")

    def uncomplete(self, taskN):
        mark = "✓"
        try:
            with open(self.file, "r", encoding="utf-8") as f:
                data = json.load(f)
        except FileNotFoundError:
            print("json não encontrado")
            return

        for entry in data:
            for i, task in enumerate(entry["txt"]):
                if task.startswith(f"{mark} {taskN}"):
                    entry["txt"][i] = taskN

        with open(self.file, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=4, ensure_ascii=False)

        print(f"'{taskN}' reniciada!")
    def registry(self):
        path = r"SOFTWARE\Microsoft\Windows\CurrentVersion\Run"
        name = "Todo" 
        val = os.path.join(os.path.dirname(os.path.abspath(__file__)), "todo_overlay.exe")
        try:
            key = winreg.OpenKey(winreg.HKEY_CURRENT_USER, path, 0, winreg.KEY_SET_VALUE | winreg.KEY_CREATE_SUB_KEY)
            winreg.SetValueEx(key, "todo", 0, winreg.REG_SZ, val)
            print(f"{val} salvado com sucesso")
            winreg.CloseKey(key)
        except Exception as e:
            print(f"Erro: {e}")
            
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="TODO manager")
    parser.add_argument("--create",action="store_true", help="crie o data.json inicial")
    parser.add_argument("--new",action="store_true", help="adicione mais um objeto")
    parser.add_argument("--reg",action="store_true", help="iniciar com o windows ")
    parser.add_argument("--complete", help="marque uma tarefa como completa")
    parser.add_argument("--uncomplete", help="desmarque uma tarefa")

    args = parser.parse_args()

    mgr = manager("data.json")
    if args.create:
        mgr.create_initial_object()
        mgr.add_object()
    elif args.new:
        mgr.add_object()
    elif args.complete:
        mgr.complete(args.complete)
    elif args.uncomplete:
        mgr.uncomplete(args.uncomplete)
    elif args.reg:
        mgr.registry()
   
