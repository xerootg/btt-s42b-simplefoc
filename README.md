# BigTreeTech S42B 2.0 SimpleFOC

This is the S42B 2.0 board running SimpleFOC!

I threw togther a sensor for the TLE5012B and have a really _really_ rough running stepper. Atleast there's reasonable feedback.

Basically, you hook up a stlink and a uart bridge to the thing and run `pio run -t upload` to make the magic happen.

## TODO
* make the stepper run smooth. I've heard rumors of needing overclocking to make this happen on the s42b derrived firmware, so maybe here too.
* get SimpleFOCCommander to work reasonably for PID tuning
* implement Step/Dir interface (should honestly be just a line or two, there's already code for this in SimpleFOC)
* figure out current regulation.
* tune positional PID on something!