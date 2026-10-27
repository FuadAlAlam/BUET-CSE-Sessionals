import tkinter as tk
import numpy as np
import math
from discrete_framework import DiscreteSignal, DFTAnalyzer, FastFourierTransform

class DoodlingApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Fourier Epicycles Doodler")
        
        # --- UI Layout ---
        self.canvas = tk.Canvas(root, width=800, height=600, bg="white")
        self.canvas.pack()
        
        control_frame = tk.Frame(root)
        control_frame.pack(pady=10)
        
        # Buttons
        tk.Button(control_frame, text="Clear Canvas", command=self.clear).pack(side=tk.LEFT, padx=5)
        tk.Button(control_frame, text="Draw Epicycles", command=self.run_transform).pack(side=tk.LEFT, padx=5)
        
        # Toggle Switch (Radio Buttons)
        self.use_fft = tk.BooleanVar(value=False)
        tk.Label(control_frame, text=" |  Algorithm: ").pack(side=tk.LEFT, padx=5)
        tk.Radiobutton(control_frame, text="Naive DFT", variable=self.use_fft, value=False).pack(side=tk.LEFT)
        tk.Radiobutton(control_frame, text="FFT", variable=self.use_fft, value=True).pack(side=tk.LEFT)

        # State Variables
        self.points = []
        self.drawing = False
        self.fourier_coeffs = None
        self.is_animating = False
        self.after_id = None
        self.prev_tip = None
        self.sorted_idx = None
        self.N = 0

        # Bindings
        self.canvas.bind("<Button-1>", self.start_draw)
        self.canvas.bind("<B1-Motion>", self.draw)
        self.canvas.bind("<ButtonRelease-1>", self.end_draw)

    def start_draw(self, event):
        self.is_animating = False
        if self.after_id:
            self.root.after_cancel(self.after_id)
        self.canvas.delete("all")
        self.points = []
        self.drawing = True

    def draw(self, event):
        if self.drawing:
            x, y = event.x, event.y
            self.points.append((x, y))
            r = 2
            self.canvas.create_oval(x-r, y-r, x+r, y+r, fill="black", outline="black")

    def end_draw(self, event):
        self.drawing = False

    def clear(self):
        self.canvas.delete("all")
        self.points = []
        self.is_animating = False
        if self.after_id:
            self.root.after_cancel(self.after_id)

    def draw_epicycle(self, x, y, radius):
        """
        Helper method for students to draw a circle (epicycle).
        x, y: Center coordinates
        radius: Radius of the circle
        """
        self.canvas.create_oval(x-radius, y-radius, x+radius, y+radius, outline="blue", tags="epicycle")

    def run_transform(self):
        if not self.points: return
        
        mean_x = sum(p[0] for p in self.points)/len(self.points)
        mean_y = sum(p[1] for p in self.points)/len(self.points)
        
        complex_pts = []
        for px, py in self.points:
            complex_pts.append((px-mean_x)+1j*(py-mean_y))
        
        signal = DiscreteSignal(complex_pts)

        if self.use_fft.get():
            analyzer = FastFourierTransform()
            n = len(signal)
            new_n = 1
            while new_n < n:
                new_n *= 2
            signal = signal.interpolate(new_n)
        else:
            analyzer = DFTAnalyzer()

        self.fourier_coeffs = analyzer.compute_dft(signal)
        self.N = len(signal)

        magnitudes = []
        for k in range(self.N):
            magnitudes.append((abs(self.fourier_coeffs[k]), k))
        magnitudes.sort(reverse=True)
        self.sorted_idx = [m[1] for m in magnitudes]

        self.canvas.delete("all")
        self.prev_tip = None
        self.animate_epicycles((mean_x, mean_y))

    def animate_epicycles(self, center_offset):
        self.is_animating = True
        self.time_step = 0
        self.center_offset = center_offset
        self.update_frame()

    def update_frame(self):
        if not self.is_animating: return
        
        self.canvas.delete("epicycle") 
        
        N = self.N
        t = self.time_step
        coeffs = self.fourier_coeffs

        x = self.center_offset[0]
        y = self.center_offset[1]

        for k in self.sorted_idx:
            ck = coeffs[k]/N
            rad = abs(ck)
            if rad < 0.5:
                continue
            
            self.draw_epicycle(x, y, rad)
            
            angle = 2*math.pi*k*t/N
            val = ck*(math.cos(angle)+1j*math.sin(angle))
            dx = val.real
            dy = val.imag

            self.canvas.create_line(x, y, x+dx, y+dy, fill="blue", tags="epicycle")
            x += dx
            y += dy


        r = 3
        self.canvas.create_oval(x-r, y-r, x+r, y+r, fill="red", outline="red", tags="epicycle")
        
        if self.prev_tip is not None:
            self.canvas.create_line(self.prev_tip[0], self.prev_tip[1], x, y, fill="red", width=2, tags="trace")
        self.prev_tip = (x, y)

        self.time_step = (self.time_step + 1)

        if self.time_step >= N:
            self.time_step = 0
            self.canvas.delete("trace")
            self.prev_tip = None
        
        self.after_id = self.root.after(50, self.update_frame)

if __name__ == "__main__":
    root = tk.Tk()
    app = DoodlingApp(root)
    root.mainloop()