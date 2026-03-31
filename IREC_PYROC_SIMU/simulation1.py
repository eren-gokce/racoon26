"""
simulation1.py — Active Roll Control Simulation
=================================================
Copies the base rocket simulation from simulation.py, then runs a SEPARATE
roll-axis ODE integration to simulate:
  1) Fake rotation impurities  (fluctuating torque, 0–1 N·m)
  2) PID-controlled active fins that fight the unwanted roll

WHY SEPARATE?  RocketPy v1.12 uses an internal compiled ODE solver.
Overriding u_dot in Python has NO effect.  So we:
  • Run the normal RocketPy flight to get trajectory data (velocity, etc.)
  • Run our own scipy.integrate.solve_ivp for the roll channel only.

TOGGLE:  Set ENABLE_PID = True or False on line 24 to compare.
"""

import math
import numpy as np
import matplotlib.pyplot as plt
from scipy.integrate import solve_ivp
from rocketpy import Environment, SolidMotor, Rocket, Flight
import datetime

# ==================== CONTROL TOGGLE ====================
# Set to False first to see fake rotation only, then True to see PID fix it
ENABLE_PID = True
# =========================================================


# ===================== PID Controller ====================
class PIDController:
    def __init__(self, Kp, Ki, Kd, setpoint=0.0, max_torque=5.0):
        self.Kp = Kp
        self.Ki = Ki
        self.Kd = Kd
        self.setpoint = setpoint
        self.max_torque = max_torque  # servo physical limit (N·m)

    def compute(self, error, integral, derivative):
        """Pure function — no hidden state.  Safe for ODE solvers."""
        torque = self.Kp * error + self.Ki * integral + self.Kd * derivative
        return np.clip(torque, -self.max_torque, self.max_torque)


# ============= Fake Impurity Torque Function =============
def impurity_torque(t):
    """Fluctuating roll disturbance between 0 and 1 N·m.
    Uses a squared sine so the value is always non-negative."""
    return math.sin(math.pi * 0.5 * t) *2
    # #import random
    # if t <= 50 or t >= 500:
    #     # return round(random.uniform(0, 1), 4)
    #     return 1
    # if t > 50 and t <= 100:
    #     # return round(random.uniform(0, 1), 4)
    #     return -3
    # if t > 100 and t <= 150:
    #     # return round(random.uniform(0, 1), 4)
    #     return -1
    # if t > 150 and t <= 200:
    #     # return round(random.uniform(0, 1), 4)
    #     return 1.4
    # if t > 200:
    #     # return round(random.uniform(0, 1), 4)
    #     return 2.5

# =============== Roll Dynamics ODE ========================
def roll_ode(t, y, I_zz, pid, enable_pid):
    """
    State:  y = [theta, omega, integral_error]
      theta  — roll angle  (rad)
      omega  — roll rate   (rad/s)
      int_e  — integral of roll-rate error (for PID I-term)

    Equation:  I_zz * d(omega)/dt = T_impurity + T_pid
    """
    theta, omega, int_e = y

    # --- disturbance ---
    T_imp = impurity_torque(t)

    # --- PID control ---
    T_pid = 0.0
    error = pid.setpoint - omega          # we want omega → 0
    d_int_e = error                        # will be integrated by the solver
    derivative = -T_imp / I_zz             # simple estimate of d(error)/dt

    if enable_pid:
        T_pid = pid.compute(error, int_e, derivative)

    alpha = (T_imp + T_pid) / I_zz         # angular acceleration

    return [omega, alpha, d_int_e]


# ================== ROCKET DEFINITION ====================
# (exact copy of simulation.py)

tomorrow = datetime.date.today() + datetime.timedelta(days=1)

env = Environment(latitude=31.956944, longitude=-102.201667, elevation=878)
env.set_date(
    (tomorrow.year, tomorrow.month, tomorrow.day, 12)
)  # Hour given in UTC time

# NOTE: GFS Forecast is currently broken (NOAA retired OpenDAP — SCN 25-81).
# Uncomment below when GFS comes back online:
# env.set_atmospheric_model(type="Forecast", file="GFS")
env.set_atmospheric_model(type="standard_atmosphere")

M1850W = SolidMotor(
    thrust_source="AeroTech_M1850W.eng",
    dry_mass=2.892,
    dry_inertia=(0.125, 0.125, 0.002),
    nozzle_radius=28.6 / 1000,
    grain_number=6,
    grain_density=1700,
    grain_outer_radius=31.5 / 1000,
    grain_initial_inner_radius=12.7 / 1000,
    grain_initial_height=121 / 1000,
    grain_separation=1.5 / 1000,
    grains_center_of_mass_position=0.485,
    center_of_dry_mass_position=0.461,
    nozzle_position=0,
    burn_time=4,
    throat_radius=11.1 / 1000,
    coordinate_system_orientation="nozzle_to_combustion_chamber",
)

calisto = Rocket(
    radius=124 / 2000,
    mass=15.922,
    inertia=(6.321, 6.321, 0.034),
    power_off_drag="drag_curve.csv",
    power_on_drag="drag_curve.csv",
    center_of_mass_without_motor=1.238,
    coordinate_system_orientation="nose_to_tail",
)

calisto.add_motor(M1850W, position=2.730)

rail_buttons = calisto.set_rail_buttons(
    upper_button_position=1.18,
    lower_button_position=2.03,
    angular_position=45,
)

nose_cone = calisto.add_nose(
    length=0.3, kind="parabolic", position=0
)

fin_set = calisto.add_trapezoidal_fins(
    n=4,
    root_chord=0.3,
    tip_chord=0.09,
    span=0.09,
    position=2.360,
    cant_angle=0,
    sweep_length=0.269,
)

main = calisto.add_parachute(
    name="main",
    cd_s=3.928,
    trigger=400,
    sampling_rate=105,
    lag=1.5,
    noise=(0, 8.3, 0.5),
    radius=1.25,
    height=3,
    porosity=0.0432,
)

drogue = calisto.add_parachute(
    name="drogue",
    cd_s=0.63,
    trigger="apogee",
    sampling_rate=105,
    lag=1.5,
    noise=(0, 8.3, 0.5),
    radius=0.5,
    height=1,
    porosity=0.0432,
)

# ================== RUN BASE FLIGHT ====================
print("Running base RocketPy flight...")
test_flight = Flight(
    rocket=calisto, environment=env, rail_length=5.180, inclination=84, heading=90
)
flight_time = test_flight.t_final
print(f"Flight time: {flight_time:.1f} s")

# =========== RUN ROLL-AXIS SIMULATION ==================
# Roll inertia  (I_zz from the rocket definition)
I_zz = 0.034  # kg·m²  (same as calisto inertia[2])

pid = PIDController(Kp=10, Ki=0.2, Kd=0.03, setpoint=0.0, max_torque=5.0)

print(f"Running roll simulation (PID {'ON' if ENABLE_PID else 'OFF'})...")

t_span = (0, flight_time)
t_eval = np.linspace(0, flight_time, 2000)
y0 = [0.0, 0.0, 0.0]  # theta=0, omega=0, integral=0

sol = solve_ivp(
    roll_ode,
    t_span,
    y0,
    args=(I_zz, pid, ENABLE_PID),
    t_eval=t_eval,
    method="RK45",
    max_step=0.01,
)

times = sol.t
theta = sol.y[0]   # roll angle (rad)
omega = sol.y[1]   # roll rate  (rad/s)

# Recompute torques for plotting
T_imp_arr = np.array([impurity_torque(t) for t in times])
T_pid_arr = np.zeros_like(times)
if ENABLE_PID:
    for i, (t, w, ie) in enumerate(zip(times, omega, sol.y[2])):
        error = pid.setpoint - w
        derivative = -T_imp_arr[i] / I_zz
        T_pid_arr[i] = pid.compute(error, ie, derivative)

# ===================== PLOTTING ========================
fig, axes = plt.subplots(3, 1, figsize=(12, 10), sharex=True)

# 1) Roll rate
axes[0].plot(times, np.degrees(omega), color="dodgerblue", linewidth=1.2)
axes[0].set_ylabel("Roll Rate (°/s)")
axes[0].set_title(f"Roll Rate — PID {'ON' if ENABLE_PID else 'OFF'}")
axes[0].grid(True, alpha=0.3)
axes[0].axhline(0, color="gray", linewidth=0.5)

# 2) Roll angle
axes[1].plot(times, np.degrees(theta), color="darkorange", linewidth=1.2)
axes[1].set_ylabel("Roll Angle (°)")
axes[1].set_title("Cumulative Roll Angle")
axes[1].grid(True, alpha=0.3)

# 3) Torques
axes[2].plot(times, T_imp_arr, color="red", alpha=0.8, linewidth=1.0, label="Impurity Torque")
if ENABLE_PID:
    axes[2].plot(times, T_pid_arr, color="green", alpha=0.8, linewidth=1.0, label="PID Torque")
    axes[2].plot(times, T_imp_arr + T_pid_arr, color="purple", alpha=0.6, linewidth=1.0, linestyle="--", label="Net Torque")
axes[2].set_ylabel("Torque (N·m)")
axes[2].set_xlabel("Time (s)")
axes[2].set_title("Torques Acting on Roll Axis")
axes[2].legend()
axes[2].grid(True, alpha=0.3)

plt.tight_layout()
filename = f"roll_control_pid_{'on' if ENABLE_PID else 'off'}.png"
plt.savefig(filename, dpi=150)
print(f"Saved: {filename}")
plt.show()

test_flight.plots.all()
