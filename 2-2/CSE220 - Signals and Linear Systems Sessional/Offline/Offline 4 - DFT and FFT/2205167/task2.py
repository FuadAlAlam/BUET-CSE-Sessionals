import tkinter as tk
from tkinter import filedialog, messagebox
import numpy as np
import scipy.io.wavfile as wav
import sounddevice as sd
from discrete_framework import DiscreteSignal, DFTAnalyzer, FastFourierTransform

class AudioEqualizer:
    def __init__(self, root):
        self.root = root
        self.root.title("DFT Audio Equalizer")
        
        self.samplerate = 0
        self.original_audio = None
        self.processed_audio = None
        
        # --- UI Layout ---
        top_frame = tk.Frame(root)
        top_frame.pack(pady=10)
        
        tk.Button(top_frame, text="Load WAV File", command=self.load_file).pack(side=tk.LEFT, padx=10)
        tk.Button(top_frame, text="Process & Play", command=self.process_and_play).pack(side=tk.LEFT, padx=10)
        tk.Button(top_frame, text="Stop Audio", command=sd.stop).pack(side=tk.LEFT, padx=10)
        
        # Toggle Switch
        control_frame = tk.Frame(root)
        control_frame.pack(pady=5)
        self.use_fft = tk.BooleanVar(value=False)
        tk.Label(control_frame, text="Algorithm: ").pack(side=tk.LEFT)
        tk.Radiobutton(control_frame, text="DFT (Slow)", variable=self.use_fft, value=False).pack(side=tk.LEFT)
        tk.Radiobutton(control_frame, text="FFT (Fast)", variable=self.use_fft, value=True).pack(side=tk.LEFT)

        # Equalizer Sliders
        self.slider_frame = tk.Frame(root)
        self.slider_frame.pack(pady=20, padx=20)
        
        self.sliders = []
        labels = ["Low", "Low-Mid", "Mid", "High-Mid", "High"]
        for i in range(5):
            frame = tk.Frame(self.slider_frame)
            frame.pack(side=tk.LEFT, padx=5)
            tk.Label(frame, text=labels[i], font=("Arial", 8)).pack()
            slider = tk.Scale(frame, from_=2.0, to=0.0, resolution=0.1, length=150, orient=tk.VERTICAL)
            slider.set(1.0)
            slider.pack()
            self.sliders.append(slider)

    def load_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("WAV files", "*.wav")])
        if file_path:
            try:
                self.samplerate, data = wav.read(file_path)
                
                # normalize to float [-1, 1]
                if data.dtype == np.int16:
                    data = data.astype(np.float32) / 32768.0
                elif data.dtype == np.int32:
                    data = data.astype(np.float32) / 2147483648.0
                elif data.dtype == np.uint8:
                    data = (data.astype(np.float32) - 128.0) / 128.0
                
                # If already float, just ensure float32
                if data.dtype != np.float32:
                    data = data.astype(np.float32)

                # Convert to mono if stereo
                if len(data.shape) > 1:
                    data = np.mean(data, axis=1)
                
                self.original_audio = data
                self.processed_audio = None
                duration = len(data) / self.samplerate
                print(f"Loaded: {len(data)} samples, {self.samplerate} Hz, {duration:.1f}s")
            except Exception as e:
                messagebox.showerror("Error", f"Could not load file: {e}")

    def process_and_play(self):
        if self.original_audio is None:
            messagebox.showwarning("Warning", "Please load a WAV file first.")
            return
        
        print("Starting processing...")
        # Get Slider Values
        gains = [s.get() for s in self.sliders]
        
        if self.use_fft.get():
            analyzer = FastFourierTransform()
        else:
            analyzer = DFTAnalyzer()

        audio = self.original_audio
        total = len(audio)
        chunk_size = 1024

        output = np.zeros(total, dtype=np.float64)
        num_chunks = (total+chunk_size-1) // chunk_size

        for i in range(num_chunks):
            start = i*chunk_size
            end = start+chunk_size
            if end > total:
                end = total
            chunk = audio[start:end]
            real_len = len(chunk)

            if real_len < chunk_size:
                padded = np.zeros(chunk_size, dtype=np.float32)
                padded[:real_len] = chunk
                chunk = padded

            sig = DiscreteSignal(chunk)
            spectrum = analyzer.compute_dft(sig)

            N = len(spectrum)
            half = N // 2
            for k in range(N):
                if k <= half:
                    freq_idx = k
                else:
                    freq_idx = N - k
                band = (freq_idx*5) // half
                if band > 4:
                    band = 4
                spectrum[k] = spectrum[k]*gains[band]

            result = analyzer.compute_idft(spectrum)

            output[start:end] = result[:real_len].real

            if (i+1)%50 == 0 or (i+1) == num_chunks:
                print(f"  Processed chunk {i+1}/{num_chunks}")

        peak = np.max(np.abs(output))
        if peak > 0:
            output = output/peak

        self.processed_audio = output.astype(np.float32)

        print("Processing complete. Playing audio...")
        sd.stop()
        sd.play(self.processed_audio, self.samplerate)


if __name__ == "__main__":
    root = tk.Tk()
    app = AudioEqualizer(root)
    root.mainloop()