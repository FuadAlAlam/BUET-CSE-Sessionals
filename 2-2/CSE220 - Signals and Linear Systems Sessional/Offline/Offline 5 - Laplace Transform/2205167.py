import numpy as np
import matplotlib.pyplot as plt

class SmartIrrigation:
    def __init__(self, a=0.5, b=1.0, t_max=20, dt=0.01):
        self.a = a
        self.b = b
        self.t_max = t_max
        self.dt = dt
        self.t = np.arange(0, t_max, dt)

    def u_step(self):
        return np.ones_like(self.t)
    
    def u_ramp(self):
        return 0.1*self.t

    def u_sin(self):
        return np.sin(0.5*self.t)
    
    def u_exponential(self): 
        return 1 - np.exp(-0.3*self.t)
    
    def u_pulse(self):
        return np.where((self.t >= 0) & (self.t < 5), 1.0, 0.0)

    def laplace_transform(self, f, s):
        integrand = f*np.exp(-s*self.t)
        integral = np.sum(0.5*self.dt*(integrand[:-1] + integrand[1:]))

        return integral

    def inverse_laplace(self, s_list, H_s_values):
        h_t = np.zeros_like(self.t)

        if len(s_list)>1:
            d_omega = np.imag(s_list[1] - s_list[0])
        else:
            d_omega = 0
        
        for i, t_val in enumerate(self.t):
            sum_val = np.sum(H_s_values*np.exp(s_list*t_val))
            h_t[i] = (d_omega/(2*np.pi))*np.real(sum_val)
            
        return h_t

    def H_s(self, s, U_s):
        return (self.b/(s+self.a))*U_s
    
    def steady_state(self, h):
        """Mean of last 5% of signal."""
        idx = int(0.05*len(h))
        if idx == 0:
            idx = 1
        return np.mean(h[-idx:])

    def time_constant(self, h):
        """Time to first reach 63.2% of steady-state."""
        h_ss = self.steady_state(h)
        if abs(h_ss) < 1e-4:
            return 0.0
        
        target = 0.632*h_ss
        
        if h_ss > 0:
            idx = np.where(h >= target)[0]
        else:
            idx = np.where(h <= target)[0]
        
        if len(idx)>0:
            return self.t[idx[0]]
        
        return 0.0

    def rise_time(self, h):
        """Time to go from 10% to 90% of steady-state."""
        h_ss = self.steady_state(h)
        if abs(h_ss) < 1e-4:
            return 0.0
        
        if h_ss > 0:
            idx_10 = np.where(h >= 0.1*h_ss)[0]
            idx_90 = np.where(h >= 0.9*h_ss)[0]
        else:
            idx_10 = np.where(h <= 0.1*h_ss)[0]
            idx_90 = np.where(h <= 0.9*h_ss)[0]
            
        if len(idx_10)>0 and len(idx_90)>0:
            return self.t[idx_90[0]] - self.t[idx_10[0]]
        
        return 0.0

    def settling_time(self, h):
        """Time after which h(t) stays permanently within ±2% of h_ss."""
        h_ss = self.steady_state(h)
        if abs(h_ss) < 1e-4:
            return 0.0
        
        ub = h_ss + abs(0.02*h_ss)
        lb = h_ss - abs(0.02*h_ss)
        
        out_of_bounds = np.where((h>ub) | (h<lb))[0]
        
        if len(out_of_bounds) == 0:
            return 0.0
            
        last_idx = out_of_bounds[-1]
        if last_idx+1 < len(self.t):
            return self.t[last_idx+1]
        
        return self.t[-1]

    def overshoot(self, h):
        """Percentage overshoot: (h_max - h_ss) / h_ss * 100."""
        h_ss = self.steady_state(h)
        if abs(h_ss) < 1e-4:
            return 0.0
        
        h_max = np.max(h)
        if h_max > h_ss and h_ss>0:
            return ((h_max-h_ss)/h_ss)*100.0
        
        return 0.0

    def compute_metrics(self, h):
       
        return {
            "steady_state":  self.steady_state(h),
            "time_constant": self.time_constant(h),
            "rise_time":     self.rise_time(h),
            "settling_time": self.settling_time(h),
            "overshoot_%":   self.overshoot(h),
        }

    def euler_simulate(self, u):
        """
        Euler method for dh/dt = -a*h(t) + b*u(t)
        h[n+1] = h[n] + dt * (-a*h[n] + b*u[n])
        """
        h = np.zeros_like(self.t)
        for n in range(len(self.t) - 1):
            dhdt = -self.a * h[n] + self.b * u[n]
            h[n + 1] = h[n] + self.dt * dhdt
        return h


def analyze_parameter_variation(a, b, plot_label):
    print(f"Testing Parameters: a = {a} (outflow), b = {b} (input gain)")
    
    sys_var = SmartIrrigation(a=a, b=b, t_max=20, dt=0.01)
    
    u_step = sys_var.u_step() 
    h_var = sys_var.euler_simulate(u_step) 

    print("Performance Metrics:")
    metrics = sys_var.compute_metrics(h_var)
    for k, v in metrics.items():
        print(f"  {k.replace('_',' ').title():<22}: {v}")

            
    plt.figure(figsize=(8, 4.5))
    plt.plot(sys_var.t, u_step, 'b--', lw=1.5, label="Step Input u(t)=1")
    plt.plot(sys_var.t, h_var, color='#FF5722', lw=2.5, label=f"Output h(t)")
    plt.title(f"{plot_label} (a={a}, b={b})", fontweight='bold')
    plt.xlabel("Time (s)")
    plt.ylabel("Water Level h(t)")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.show()



#Change values of a, b to experiment with different system dynamics
system = SmartIrrigation(a=0.5, b=1.0, t_max=20, dt=0.01)

inputs = {
    "Step Input":        system.u_step(),
    "Ramp Input":        system.u_ramp(),
    "Sinusoidal Input":  system.u_sin(),
    "Exponential Input": system.u_exponential(),
    "Pulse Input":       system.u_pulse(),
}

# Bromwich contour parameters, set these values
c = 0.5
W = 100
N = 2000

dw = 2*W/N
omega_k = np.zeros(N)
for k in range(N):
    omega_k[k] = -W + k*dw
s_list = c + 1j*omega_k

colors = ['#2196F3', '#4CAF50', '#FF5722', '#9C27B0', '#FF9800']

for idx, (name, u) in enumerate(inputs.items()):
    print(f"Processing: {name}...")

    # --- Laplace --- set these values
    U_s_vals = np.zeros(len(s_list), dtype=complex)

    for i in range(len(s_list)):
        cur_s = s_list[i]
        U_s_vals[i] = system.laplace_transform(u, cur_s)
    
    H_s_vals = system.H_s(s_list, U_s_vals)
    h_laplace = system.inverse_laplace(s_list, H_s_vals)


    print(f"\n  ► {name}")

    metrics = system.compute_metrics(h_laplace)
    
    for k, v in metrics.items():
        print(f"      {k.replace('_',' ').title():<22}: {v}")

    # --- Euler ---
    h_euler = system.euler_simulate(u)

    # --- Plot ---
    fig, axes = plt.subplots(1, 2, figsize=(14, 5), sharey=False)
    fig.suptitle(f"Smart Irrigation — {name}", fontsize=13, fontweight='bold')

    # Laplace subplot
    axes[0].plot(system.t, u, 'b--', lw=1.8, label="Input u(t)")
    axes[0].plot(system.t, h_laplace, color=colors[idx], lw=2.2, label="Output h(t)")
    axes[0].set_title("Laplace Transform Simulation", fontweight='bold')
    axes[0].set_xlabel("Time (s)", fontsize=11)
    axes[0].set_ylabel("Water Level / Input", fontsize=11)
    axes[0].legend(fontsize=10)
    axes[0].grid(True, alpha=0.3)

    # Euler subplot
    axes[1].plot(system.t, u, 'b--', lw=1.8, label="Input u(t)")
    axes[1].plot(system.t, h_euler, color='tomato', lw=2.2, label="Output h(t)")
    axes[1].set_title("Euler Method Simulation", fontweight='bold')
    axes[1].set_xlabel("Time (s)", fontsize=11)
    axes[1].set_ylabel("Water Level / Input", fontsize=11)
    axes[1].legend(fontsize=10)
    axes[1].grid(True, alpha=0.3)

    plt.tight_layout()
    plt.show()



analyze_parameter_variation(0.5, 1.0, "Default")
analyze_parameter_variation(1.0, 1.0, "Faster Drain")
analyze_parameter_variation(0.5, 2.0, "Higher Gain")
analyze_parameter_variation(0.2, 0.5, "Slow System")