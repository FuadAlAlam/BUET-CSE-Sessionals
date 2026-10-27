import numpy as np

class DiscreteSignal:
    """
    Represents a discrete-time signal.
    """
    def __init__(self, data):
        # Ensure data is a numpy array, potentially complex
        self.data = np.array(data, dtype=np.complex128)

    def __len__(self):
        return len(self.data)
        
    def pad(self, new_length):
        """
        Zero-pad or truncate signal to new_length.
        Returns a new DiscreteSignal object.
        """
        if new_length <= len(self.data):
            return DiscreteSignal(self.data[:new_length])
        padded = np.zeros(new_length, dtype=np.complex128)
        for i in range(len(self.data)):
            padded[i] = self.data[i]
        return DiscreteSignal(padded)

    def interpolate(self, new_length):
        """
        Resample signal to new_length using linear interpolation.
        Required for Task 4 (Drawing App).
        """
        n = len(self.data)
        if n == 0:
            return DiscreteSignal(np.zeros(new_length, dtype=np.complex128))
        if n == new_length:
            return DiscreteSignal(self.data.copy())
        
        old_x = np.arange(n)
        new_x = np.linspace(0, n-1, new_length)
        re = np.interp(new_x, old_x, self.data.real)
        im = np.interp(new_x, old_x, self.data.imag)
        return DiscreteSignal(re+1j*im)


class DFTAnalyzer:
    """
    Performs Discrete Fourier Transform using O(N^2) method.
    """
    def compute_dft(self, signal):
        """
        Compute DFT using naive summation.
        Returns: numpy array of complex frequency coefficients.
        """
        N = len(signal)
        X = np.zeros(N, dtype=np.complex128)
        n = np.arange(N)
        for k in range(N):
            X[k] = np.sum(signal.data * np.exp(-2j*np.pi*k*n/N))
        return X

    def compute_idft(self, spectrum):
        """
        Compute Inverse DFT using naive summation.
        Returns: numpy array (time-domain samples).
        """
        spectrum = np.array(spectrum, dtype=np.complex128)
        N = len(spectrum)
        x = np.zeros(N, dtype=np.complex128)
        k = np.arange(N)
        for n in range(N):
            x[n] = np.sum(spectrum * np.exp(2j*np.pi*k*n/N))
        x = x/N
        return x


class FastFourierTransform(DFTAnalyzer):
    def compute_dft(self, signal):
        return self._fft(signal.data.copy())

    def compute_idft(self, spectrum):
        spectrum = np.array(spectrum, dtype=np.complex128)
        N = len(spectrum)
        result = self._fft(np.conj(spectrum))
        return np.conj(result)/N

    def _fft(self, x):
        N = len(x)
        if N <= 1:
            return x.copy()
        
        if N%2 != 0:
            return super().compute_dft(DiscreteSignal(x))

        even = self._fft(x[0::2])
        odd = self._fft(x[1::2])

        T = np.zeros(N // 2, dtype=np.complex128)
        for m in range(N // 2):
            T[m] = np.exp(-2j*np.pi*m/N)*odd[m]

        result = np.zeros(N, dtype=np.complex128)
        for m in range(N // 2):
            result[m] = even[m] + T[m]
            result[m + N // 2] = even[m] - T[m]
        return result