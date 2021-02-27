import tkinter as tk
from tkinter import ttk


class ScrollableFrame(tk.Frame):
    def __init__(self, container, *args, **kwargs):
        super().__init__(container, *args, **kwargs)
        canvas = tk.Canvas(self,highlightthickness=0,bg=kwargs['bg'],width=kwargs['width'],height=100)
        scrollbar = ttk.Scrollbar(self, orient="vertical", command=canvas.yview)
        self.scrollable_frame = tk.Frame(canvas)
        self.add_height=0

        # self.scrollable_frame.bind(
        #     "<Configure>",
        #     lambda e: canvas.configure(
        #         scrollregion=canvas.bbox("all")
        #     )
        # )

        def ExpandCanvas(e, c):
            h = e.height
            h=int(h)
            # print(h)
            # print(e.height)
            # print(c.winfo_height())
            if h < 350:
                h = e.height
            else:
                h = 350
            # print(h)
            c.configure(scrollregion=c.bbox('all'), height=h, width=e.width)

        self.scrollable_frame.bind(
            "<Configure>",
            lambda e: ExpandCanvas(e,canvas)
        )


        canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")

        canvas.configure(yscrollcommand=scrollbar.set)

        canvas.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")

