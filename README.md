Hello, welcome to my project.

CDrumInstrument Component

Functionality:
Generates synthesized drum sounds for various types: bass drum, snare, tom, and cymbal.
Uses different synthesis techniques for each drum type, such as low-frequency oscillation (LFO) for bass and noise-based generation for snare.
Implements ADSR envelope control over time to mimic realistic decay for drum sounds.
Works with CSynthesizer to enable drum sound playback and integrates XML configuration for drum type selection.
Ownership: Orhan Aydin



CFlangerEffect Component

Functionality:
Implements a flanger effect by using a low-frequency oscillator (LFO) to modulate delay times.
Includes adjustable parameters: modulation depth, rate, feedback, and wet/dry mix.
Processes each audio sample with a delay buffer, adding delayed and original samples for the flanger effect.
Modulates delay times based on sine wave LFO and includes feedback for the classic flanger effect.
Ownership: Orhan Aydin




CChorusEffect Component

Functionality:
Creates a chorus effect by modulating multiple delayed copies of the original signal with different rates and depths.
Adds a rich, multi-voice effect by simulating multiple voices.
Adjustable parameters include depth, rate, and wet/dry mix, controlled via CSynthesizer.
Ownership: Orhan Aydin




Noise Gating and Compression/Limiting Effects

Functionality:
Noise Gate: Suppresses audio signals below a specified threshold, allowing only signals above the threshold to pass, reducing background noise.
Compression/Limiting: Controls audio signal dynamics by reducing peaks to prevent distortion. It ensures a more uniform volume by limiting extreme peaks while enhancing quieter signals.
Ownership: Orhan Aydin

