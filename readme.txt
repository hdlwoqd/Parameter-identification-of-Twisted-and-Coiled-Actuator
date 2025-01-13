The models of Twisted and coiled Actuator (TCA) can be divided into physics-based models and control-oriented models. The control-oriented models employ parameter identification methods to determine the parameters of the TCA. The complex interplay of thermodynamic and mechanical factors leads to the nonlinear behavior of the TCA. Specifically, thermodynamics is primarily driven by Joule heating, with natural convection acting as a secondary dissipation mechanism, while radiation and damping heat further complicate the system. The mechanical response of the actuator also exhibits nonlinearity, with the force typically modeled using a linear equation that includes stiffness, damping, and temperature effects. However, more refined models account for thermal expansion, temperature-dependent elastic modulus, and hysteresis behavior.

Despite these nonlinearities, the linear dynamic model (referenced as {yip2017control}) remains widely adopted due to its simplicity, effectiveness in closed-loop control, and convenience in parameter estimation, offering comparable performance in motion control. This paper adopts this model and conducts parameter identification experiments. The project will detail the methodology for identifying the parameters of this model.

The model consists of two parts: the thermodynamic model and the mechanical model.

**Thermodynamic Model:**
\[ C_{\text{th}} \frac{dT}{dt} = P - \lambda (T - T_{\text{amb}}) \]

**Mechanical Model:**
\[ F = k(x - x_0) + b \frac{dx}{dt} + C(T - T_{\text{amb}}) \]

The parameters to be identified are \( k \), \( b \), \( C \), \( C_{\text{th}} \), and \( \lambda \).

**Identification of Parameter \( k \):** The length of the TCA was controlled using a linear motor, and the force output was measured using a force sensor. The average stiffness was determined through the force-displacement loop. In the experiment, the linear motor performed reciprocating movements, and the force and displacement were recorded at different times. The average stiffness was obtained using the least squares fitting method. The experimental results are shown in Fig.~\ref{iden}(a).

**Identification of Parameter \( b \):** The experimental results are shown in Fig.~\ref{iden}(b). A 200g mass was hung at the lower end of the TCA, and the displacement of the connected plate was measured using a laser rangefinder, indirectly reflecting the displacement of the TCA. After pulling the TCA to a predetermined position and releasing it, the system underwent damped oscillations, and the displacement data were recorded. The damping coefficient \( b \) was determined using least squares fitting.

**Identification of Parameter \( C \):** An infrared temperature sensor (MLX90640) and a thermocouple were used to measure the TCA's temperature, while a force sensor measured the output force. The TCA was heated at various power levels, and the stable temperature and output force were recorded. The experimental results are shown in Fig.~\ref{iden}(c), and the parameter \( C \) was determined using least squares fitting.

**Identification of Parameter \( \lambda \):** The experimental results are shown in Fig.~\ref{iden}(d). The infrared temperature sensor (MLX90640) and thermocouple were used to measure the TCA's temperature as it was heated at different power levels. The stable temperature was recorded, and the parameter \( \lambda \) was obtained by fitting the temperature-power data using the least squares method.

**Identification of Parameter \( C_{\text{th}} \):** The heating and cooling responses of the TCA were measured. The experimental results are shown in Fig.~\ref{iden}(e), where a laser sensor was used to measure the displacement of the TCA. Power was applied to heat the TCA until it reached a stable state, after which the power supply was stopped, and the displacement changes during the cooling process were recorded. By analyzing the data, the time constant \( \tau \) of the TCA was obtained, representing the average time required for the actuator to reach 63% of its steady-state value in response to a step input. Using the formula \( C_{\text{th}} = \lambda \times \tau \), the value of \( C_{\text{th}} \) was calculated based on the \( \tau \) value obtained from the previous experiment.

Through these experiments, the key parameters of the TCA model were successfully identified, providing a solid foundation for subsequent control and applications.
