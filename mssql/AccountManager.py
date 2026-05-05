"""
AccountManager.py - MSSQL Account Manager GUI

How to use:

- Requirements:
    - Windows
    - Python 3.8+ (supported on 3.12 in this environment)
    - `pywin32` package (for ADODB/COM access)

- Install dependencies (run in Command Prompt):

        pip install pywin32

- Run the app from the command line:

        cd "D:\TalesOfPirateDX9\TalesOfPirateDX9\mssql"
        py -3 AccountManager.py

    or, if `py` launcher is not available:

        python AccountManager.py

- Notes:
    - If using Windows Authentication (Integrated Security), run Command Prompt "As Administrator".
    - If you see Tcl/Tk errors, ensure your Python installation includes `tkinter` (standard on Windows installers).

"""

import tkinter as tk
from tkinter import messagebox, ttk
import hashlib
import binascii
import win32com.client

class AccountManagerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("MSSQL Account Manager")
        self.root.geometry("450x550")
        self.root.resizable(False, False)

        # Style
        self.style = ttk.Style()
        self.style.configure("TLabel", font=("Segoe UI", 10))
        self.style.configure("TButton", font=("Segoe UI", 10, "bold"))
        self.style.configure("Header.TLabel", font=("Segoe UI", 12, "bold"))

        # Main Container
        main_frame = ttk.Frame(root, padding="20")
        main_frame.pack(fill=tk.BOTH, expand=True)

        # SQL Connection Section
        ttk.Label(main_frame, text="SQL Connection Settings", style="Header.TLabel").grid(row=0, column=0, columnspan=2, pady=(0, 15), sticky=tk.W)

        ttk.Label(main_frame, text="Server:").grid(row=1, column=0, sticky=tk.W, pady=5)
        self.server_entry = ttk.Entry(main_frame, width=30)
        self.server_entry.insert(0, "127.0.0.1")
        self.server_entry.grid(row=1, column=1, pady=5)

        self.use_windows_auth = tk.BooleanVar(value=False)
        self.auth_check = ttk.Checkbutton(main_frame, text="Use Windows Authentication", variable=self.use_windows_auth, command=self.toggle_auth_fields)
        self.auth_check.grid(row=2, column=0, columnspan=2, pady=5, sticky=tk.W)

        self.user_label = ttk.Label(main_frame, text="SQL Username:")
        self.user_label.grid(row=3, column=0, sticky=tk.W, pady=5)
        self.user_entry = ttk.Entry(main_frame, width=30)
        self.user_entry.insert(0, "gamedb_user")
        self.user_entry.grid(row=3, column=1, pady=5)

        self.pwd_label = ttk.Label(main_frame, text="SQL Password:")
        self.pwd_label.grid(row=4, column=0, sticky=tk.W, pady=5)
        self.pwd_entry = ttk.Entry(main_frame, width=30, show="*")
        self.pwd_entry.insert(0, "passwordhere")
        self.pwd_entry.grid(row=4, column=1, pady=5)

        ttk.Separator(main_frame, orient=tk.HORIZONTAL).grid(row=5, column=0, columnspan=2, sticky=tk.EW, pady=20)

        # Account Creation Section
        ttk.Label(main_frame, text="Create New Account", style="Header.TLabel").grid(row=6, column=0, columnspan=2, pady=(0, 15), sticky=tk.W)

        ttk.Label(main_frame, text="Account Name:").grid(row=7, column=0, sticky=tk.W, pady=5)
        self.acc_name_entry = ttk.Entry(main_frame, width=30)
        self.acc_name_entry.grid(row=7, column=1, pady=5)

        ttk.Label(main_frame, text="Account Password:").grid(row=8, column=0, sticky=tk.W, pady=5)
        self.acc_pwd_entry = ttk.Entry(main_frame, width=30, show="*")
        self.acc_pwd_entry.grid(row=8, column=1, pady=5)

        # Limit account name length
        self.acc_name_entry.bind("<KeyRelease>", self.validate_length)

        # Create Button
        self.create_btn = ttk.Button(main_frame, text="CREATE ACCOUNT", command=self.create_account)
        self.create_btn.grid(row=9, column=0, columnspan=2, pady=30, ipady=5, sticky=tk.EW)

        # Status Bar
        self.status_var = tk.StringVar(value="Ready")
        status_bar = ttk.Label(main_frame, textvariable=self.status_var, relief=tk.SUNKEN, anchor=tk.W, font=("Segoe UI", 9))
        status_bar.grid(row=10, column=0, columnspan=2, sticky=tk.EW, pady=(10, 0))

    def toggle_auth_fields(self):
        state = tk.DISABLED if self.use_windows_auth.get() else tk.NORMAL
        self.user_entry.config(state=state)
        self.pwd_entry.config(state=state)

    def validate_length(self, event):
        content = self.acc_name_entry.get()
        if len(content) > 16:
            self.acc_name_entry.delete(16, tk.END)
            self.status_var.set("Warning: Max length 16 characters reached")
        else:
            self.status_var.set("Ready")

    def blake2s_hash(self, password):
        hasher = hashlib.blake2s()
        hasher.update(password.encode('utf-8'))
        digest = hasher.digest()
        return binascii.hexlify(digest).decode('utf-8').upper()

    def get_connection_string(self):
        server = self.server_entry.get()
        if self.use_windows_auth.get():
            return f"Provider=SQLOLEDB;Data Source={server};Integrated Security=SSPI;"
        else:
            user = self.user_entry.get()
            pwd = self.pwd_entry.get()
            return f"Provider=SQLOLEDB;Data Source={server};User ID={user};Password={pwd};"

    def create_account(self):
        acc_name = self.acc_name_entry.get().strip()
        acc_pwd = self.acc_pwd_entry.get().strip()

        if not acc_name or not acc_pwd:
            messagebox.showerror("Error", "Please enter both account name and password.")
            return

        if len(acc_name) > 16:
            messagebox.showerror("Error", "Account name cannot be longer than 16 characters.")
            return

        hash_pwd = self.blake2s_hash(acc_pwd)
        conn_str = self.get_connection_string()

        conn = win32com.client.Dispatch("ADODB.Connection")
        try:
            self.status_var.set("Connecting to database...")
            self.root.update_idletasks()
            conn.Open(conn_str)

            # Check if account exists
            check_sql = f"SELECT name FROM AccountServer.dbo.account_login WHERE name = '{acc_name}'"
            rs = win32com.client.Dispatch("ADODB.Recordset")
            rs.Open(check_sql, conn)
            
            if not rs.EOF:
                messagebox.showerror("Error", f"Account '{acc_name}' already exists!")
                rs.Close()
                conn.Close()
                return
            rs.Close()

            # Insert into AccountServer
            sql1 = f"INSERT INTO AccountServer.dbo.account_login (name, password) VALUES ('{acc_name}', '{hash_pwd}')"
            conn.Execute(sql1)

            # Insert into GameDB
            sql2 = f"USE GameDB; INSERT INTO account (ato_id, ato_nome, jmes) VALUES ((SELECT ISNULL(MAX(ato_id) + 1,1) FROM account), '{acc_name}', 99)"
            conn.Execute(sql2)

            messagebox.showinfo("Success", f"Account '{acc_name}' created successfully!")
            self.status_var.set(f"Successfully created account: {acc_name}")
            
            # Clear fields
            self.acc_name_entry.delete(0, tk.END)
            self.acc_pwd_entry.delete(0, tk.END)

        except Exception as e:
            messagebox.showerror("Database Error", str(e))
            self.status_var.set("Error occurred during creation.")
        finally:
            if conn.State == 1: # adStateOpen
                conn.Close()

if __name__ == "__main__":
    root = tk.Tk()
    app = AccountManagerApp(root)
    root.mainloop()
