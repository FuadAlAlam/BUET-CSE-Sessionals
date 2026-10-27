import numpy as np
import matplotlib.pyplot as plt
from imageio import imread

# =====================================================
# Continuous Image Class
# =====================================================
class ContinuousImage:
    """
    Represents an image as a continuous 2D signal.
    """

    def __init__(self, image_path):
        self.image = imread(image_path, mode='L')
        self.image = self.image / np.max(self.image)

        # Define continuous spatial axes
        self.x = np.linspace(-1, 1, self.image.shape[1])
        self.y = np.linspace(-1, 1, self.image.shape[0])

    def show(self, title="Image"):
        plt.imshow(self.image, cmap='gray')
        plt.title(title)
        plt.axis('off')
        plt.show()


# =====================================================
# 2D Continuous Fourier Transform Class
# =====================================================
class CFT2D:
    """
    Computes 2D Continuous Fourier Transform
    using separability and numerical integration.
    """

    def __init__(self, image_obj: ContinuousImage):
        self.I = image_obj.image
        self.x = image_obj.x
        self.y = image_obj.y

    def compute_cft(self):
        """
        Compute real and imaginary parts of 2D CFT.
        """
        Ny, Nx = self.I.shape

        real_p = np.zeros((Ny, Nx), dtype=float)
        imag_p = np.zeros((Ny, Nx), dtype=float)

        for yi in range(Ny):
            row = self.I[yi, :]
            for ui, u_val in enumerate(self.x):
                real_p[yi, ui] = np.trapezoid(row*np.cos(2*np.pi*u_val*self.x), self.x)
                imag_p[yi, ui] = np.trapezoid(row*np.sin(2*np.pi*u_val*self.x), self.x)

        real_uv = np.zeros((Ny, Nx), dtype=float)
        imag_uv = np.zeros((Ny, Nx), dtype=float)

        for ui in range(Nx):
            G_real = real_p[:, ui]
            G_imag = imag_p[:, ui]

            for vi, v_val in enumerate(self.y):
                cos_term = np.cos(2*np.pi*v_val*self.y)
                sin_term = np.sin(2*np.pi*v_val*self.y)

                real_uv[vi, ui] = np.trapezoid(G_real*cos_term - G_imag*sin_term, self.y)
                imag_uv[vi, ui] = np.trapezoid(- G_real*sin_term - G_imag*cos_term, self.y)

        return real_uv, imag_uv

    def plot_magnitude(self):
        """
        Plot log-scaled magnitude spectrum.
        """
        x, y = self.compute_cft()
        magnitude = np.sqrt((x ** 2) + (y ** 2))
        log_magnitude = np.log(magnitude+1)

        plt.figure()
        plt.imshow(log_magnitude, cmap='inferno', extent=[self.x[0], self.x[-1], self.y[0], self.y[-1]])
        plt.title("2D CFT Magnitude Spectrum")
        plt.axis('off') 
        plt.tight_layout()
        plt.show()


# =====================================================
# Frequency Filtering
# =====================================================
class FrequencyFilter:
    def low_pass(self, real, imag, cutoff):
        rows, cols = real.shape
        cx, cy = rows//2, cols//2

        for i in range(rows):
            for j in range(cols):
                if np.sqrt((i-cx)**2 + (j-cy)**2) > cutoff:
                    real[i,j] = 0
                    imag[i,j] = 0
        return real, imag

# =====================================================
# Inverse 2D Continuous Fourier Transform
# =====================================================
class InverseCFT2D:
    """
    Reconstructs image from 2D frequency spectrum.
    """

    def __init__(self, real, imag, x, y):
        self.real=real 
        self.imag = imag
        self.x = x
        self.y = y

    def reconstruct(self):
        """
        Perform inverse 2D CFT using numerical integration.
        """
        Ny, Nx = self.real.shape

        g_real = np.zeros((Ny, Nx), dtype=float)
        g_imag = np.zeros((Ny, Nx), dtype=float)

        for ui in range(Nx):
            col_real = self.real[:, ui]
            col_imag = self.imag[:, ui]

            for y_idx, y_val in enumerate(self.y):
                cos_term = np.cos(2*np.pi*y_val*self.y)
                sin_term = np.sin(2*np.pi*y_val*self.y)

                g_real[y_idx, ui] = np.trapezoid(col_real*cos_term - col_imag*sin_term, self.y)
                g_imag[y_idx, ui] = np.trapezoid(col_real*sin_term + col_imag*cos_term, self.y)

        reconstructed = np.zeros((Ny, Nx), dtype=float)

        for y_idx in range(Ny):
            row_real = g_real[y_idx, :]
            row_imag = g_imag[y_idx, :]

            for x_idx, x_val in enumerate(self.x):
                cos_term = np.cos(2*np.pi*self.x*x_val)
                sin_term = np.sin(2*np.pi*self.x*x_val)

                reconstructed[y_idx, x_idx] = np.trapezoid(row_real*cos_term - row_imag*sin_term, self.x)

        return reconstructed

# =====================================================
# Main Execution (Task 2)
# =====================================================
img = ContinuousImage("noisy_image.png")
img.show("Original Image")

cft2d = CFT2D(img)
real, imag = cft2d.compute_cft()
cft2d.plot_magnitude()

filt = FrequencyFilter()
real_f, imag_f = filt.low_pass(real, imag, cutoff=40)

icft2d = InverseCFT2D(real_f, imag_f, img.x, img.y)
denoised = icft2d.reconstruct()

plt.imshow(denoised, cmap='gray')
plt.title("Reconstructed (Denoised) Image")
plt.axis('off')
plt.show()
