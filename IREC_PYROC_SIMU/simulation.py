from rocketpy import Environment, SolidMotor, Rocket, Flight
import datetime

tomorrow = datetime.date.today() + datetime.timedelta(days=1)

env = Environment(latitude=31.956944, longitude=-102.201667, elevation=878)
env.set_date(
    (tomorrow.year, tomorrow.month, tomorrow.day, 12)
)  # Hour given in UTC time

env.set_atmospheric_model(type="Forecast", file="GFS")

M1850W = SolidMotor(
    thrust_source="AeroTech_M1850W.eng", ##burasi duzenlenecek motorun gercek degerlerine gore
    dry_mass=2.892, ##done
    dry_inertia=(0.125, 0.125, 0.002),
    nozzle_radius=28.6 / 1000, ##
    grain_number=6, ##
    grain_density=1700, ##
    grain_outer_radius=31.5 / 1000, ##
    grain_initial_inner_radius=12.7 / 1000, ##
    grain_initial_height=121 / 1000, ##
    grain_separation=1.5 / 1000, ##
    grains_center_of_mass_position=0.485,
    center_of_dry_mass_position=0.461,
    nozzle_position=0,
    burn_time=4, ##done
    throat_radius=11.1 / 1000, ##
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
    # airfoil=("../data/airfoils/NACA0012-radians.txt","radians"),
)

# tail = calisto.add_tail(
#     top_radius=0.0635, bottom_radius=0.0435, length=0.060, position=2.472656
# )

main = calisto.add_parachute(
    name="main",
    cd_s=3.928,
    trigger=400,      # ejection altitude in meters
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
    trigger="apogee",  # ejection at apogee
    sampling_rate=105,
    lag=1.5,
    noise=(0, 8.3, 0.5),
    radius=0.5,
    height=1,
    porosity=0.0432,
)

#env.info()
# M1850W.info()
# calisto.plots.static_margin()
# calisto.draw()

test_flight = Flight(
    rocket=calisto, environment=env, rail_length=5.180, inclination=84, heading=90
    )

# test_flight.prints.initial_conditions()
# test_flight.info()   #all of them at once

# test_flight.prints.surface_wind_conditions()
# test_flight.prints.launch_rail_conditions()
# test_flight.prints.out_of_rail_conditions()
# test_flight.prints.burn_out_conditions()
# test_flight.prints.apogee_conditions()
# test_flight.prints.events_registered()
# test_flight.prints.impact_conditions()
# test_flight.prints.maximum_values()

test_flight.plots.trajectory_3d()

# test_flight.plots.all()

from rocketpy.simulation import FlightDataExporter
FlightDataExporter(test_flight).export_kml(
    file_name="trajectory.kml",
    extrude=True,
    altitude_mode="relativetoground",
)