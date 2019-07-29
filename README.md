# BlockWorldReinforcementLearning
A Reinforcement Learning-based application for the Block World problem in a simulated environment.

The Environment is defined by a set of STRIPS-like Predicates (for example, the predicate on(0,1) indicates that the block 0 is on the block 1).
Also the Actions are in a STRIPS-like format (for example, take(0)).

The agent doesn't know the entire set of world's configuration: in fact, at the beginning, it know just the initial configuration of blocks. During the learning, going in others, inexplorated configurations, it updates the Q table.
