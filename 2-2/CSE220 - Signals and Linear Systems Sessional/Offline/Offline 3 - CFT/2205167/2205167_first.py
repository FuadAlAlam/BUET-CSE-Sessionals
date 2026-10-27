import numpy as np
import matplotlib.pyplot as plt

# =====================================================
# Abstract Base Class for Continuous-Time Signals
# =====================================================
class ContinuousSignal:
    """
    Abstract base class for all continuous-time signals.
    Every signal must be defined over a time axis t.
    """

    def __init__(self, t):
        self.t = t

    def values(self):
        """
        Returns the signal values evaluated over time axis t.
        Must be implemented by subclasses.
        """
        raise NotImplementedError("Subclasses must implement this method.")

    def plot(self, title="Signal"):
        """
        Plot the signal in the time domain.
        """
        plt.plot(self.t, self.values())
        plt.xlabel("Time (t)")
        plt.ylabel("Amplitude")
        plt.title(title)
        plt.grid(True)
        plt.show()


# =====================================================
# Signal Generator Class
# =====================================================
class SignalGenerator(ContinuousSignal):
    """
    Generates various continuous-time signals.
    Each method returns a numpy array of signal samples.
    """

    def sine(self, amplitude, frequency):
        """Generate a sine wave."""
        return amplitude*np.sin(2*np.pi*frequency*self.t)

    def cosine(self, amplitude, frequency):
        """Generate a cosine wave."""
        return amplitude*np.cos(2*np.pi*frequency*self.t)

    def square(self, amplitude, frequency):
        """Generate a square wave using sign of sine."""
        return amplitude*np.sign(np.sin(2*np.pi*frequency*self.t))

    def sawtooth(self, amplitude, frequency):
        """Generate a sawtooth wave."""
        return amplitude*2*(frequency*self.t - np.floor(0.5 + frequency*self.t))

    def triangle(self, amplitude, frequency):
        """Generate a triangle wave using arcsin formula."""
        return (2*amplitude/np.pi)*np.arcsin(np.sin(2*np.pi*frequency*self.t))

    def cubic(self, coefficient):
        """Generate a cubic polynomial signal."""
        return coefficient*(self.t ** 3)

    def parabolic(self, coefficient):
        """Generate a parabolic signal."""
        return coefficient*(self.t ** 2)

    def rectangular(self, width):
        """Generate a rectangular window centered at t=0."""
        return np.where(np.abs(self.t) <= width/2, 1.0, 0.0)

    def pulse(self, start, end):
        """Generate a finite pulse active between start and end."""
        return np.where((self.t >= start) & (self.t <= end), 1.0, 0.0)


# =====================================================
# Composite Signal Class
# =====================================================
class CompositeSignal(ContinuousSignal):
    """
    Combines multiple signals into a single composite signal.
    """

    def __init__(self, t):
        super().__init__(t)
        self.components = []

    def add_component(self, signal):
        """
        Add a signal component to the composite signal.
        """
        self.components.append(signal)

    def values(self):
        """
        Sum all signal components.
        """
        if len(self.components) == 0:
            return np.zeros_like(self.t)
        return np.sum(self.components, axis=0)


# =====================================================
# Continuous Fourier Transform Analyzer
# =====================================================
class CFTAnalyzer:
    """
    Computes the Continuous Fourier Transform (CFT)
    using numerical integration (np.trapz).
    """

    def __init__(self, signal, t, frequencies):
        self.signal = signal
        self.t = t
        self.frequencies = frequencies
        self.real = None
        self.imag = None

    def compute_cft(self):
        """
        Compute real and imaginary parts of the CFT.
        """
        x_t = self.signal.values()
        self.real = np.zeros(len(self.frequencies))
        self.imag = np.zeros(len(self.frequencies))

        for i, f in enumerate(self.frequencies):
            self.real[i] = np.trapezoid(x_t*np.cos(2*np.pi*f*self.t), self.t)
            self.imag[i] = -np.trapezoid(x_t*np.sin(2*np.pi*f*self.t), self.t)

        return self.real, self.imag

    def plot_spectrum(self):
        """
        Plot magnitude spectrum of the signal.
        """
        if self.real is None or self.imag is None:
            self.compute_cft()

        magnitude = np.sqrt(self.real ** 2 + self.imag ** 2)

        plt.figure(figsize=(10, 4))
        plt.plot(self.frequencies, magnitude)
        plt.xlabel("Frequency (Hz)")
        plt.ylabel("Magnitude")
        plt.title("CFT Magnitude Spectrum")
        plt.grid(True)
        plt.show()


# =====================================================
# Inverse Continuous Fourier Transform
# =====================================================
class InverseCFT:
    """
    Reconstructs time-domain signal using ICFT.
    """

    def __init__(self, spectrum, frequencies, t):
        self.real, self.imag = spectrum
        self.frequencies = frequencies
        self.t = t

    def reconstruct(self):
        """
        Perform inverse CFT using numerical integration.
        """
        x_reconstructed = np.zeros(len(self.t))

        for i, t_val in enumerate(self.t):
            integrand_real = (self.real*np.cos(2*np.pi*self.frequencies*t_val) - 
                              self.imag*np.sin(2*np.pi*self.frequencies*t_val))
            x_reconstructed[i] = np.trapezoid(integrand_real, self.frequencies)

        return x_reconstructed


# =====================================================
# Main Execution (Task 1)
# =====================================================
if __name__ == "__main__":
    t = np.linspace(-4, 4, 3000)
    gen = SignalGenerator(t)

    composite = CompositeSignal(t)
    composite.add_component(gen.sine(2, 1))
    composite.add_component(gen.cosine(0.5, 3))
    composite.add_component(gen.square(1, 1))
    composite.add_component(gen.cubic(1) * gen.rectangular(2))

    composite.plot("Composite Signal")

    frequencies = np.linspace(-10, 10, 1000)
    cft = CFTAnalyzer(composite, t, frequencies)
    cft.plot_spectrum()

    icft = InverseCFT(cft.compute_cft(), frequencies, t)
    x_rec = icft.reconstruct()

    plt.figure(figsize=(10, 4))
    plt.plot(t, composite.values(), label="Original")
    plt.plot(t, x_rec, '--', label="Reconstructed")
    plt.legend()
    plt.xlabel("Time (t)")
    plt.ylabel("Amplitude")
    plt.title("Reconstruction using ICFT")
    plt.grid(True)
    plt.show()