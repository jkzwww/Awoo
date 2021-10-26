FIT2097 assignment 2

Name: Zhi Wei Loh   ID: 31123074

How to play: 
- Reach the goal without dying (health turns 0) by avoiding enemies and consuming boost items
- Crack different kind of puzzles to activate door and escape facility.
- Avoid being seen by enemies which will then chase the player 
and deal damage when touched or bomb player.
- May rescue pet which can be upgraded and release useful skills to deal with enemies.

Working features:
- both previos assignment requirements and current assignment requirements are implemented
- switches including locks, flip switch , binary code , powerbox
- dynamic materials and niagara system.
- enemy AIs including a guardian ( patrol around constant path) and seeker which wanders around
- cinematics showing story and clues to the gameplay.

Extra features:
- pet added which can be rescued and then fed to upgrade.
- upgrading pet unlock skills to use against enemy including
    - charm , heal , shield skills
- laser niagara system which use line tracing and deal damage when player lasered.
- drone actors that patrol and spawn particles and deal damage when player is beneath them.
- bomber AI that send timed bombs to player as attack task.

Bugged features:
- dynamic post process not working, attempted with attaching post process component
and lerping to desired values with custom postprocess settings in C++.
- Laser actor in C++ has exact code with BP_Laser blueprint version, but niagara system
do not activate on spawn.

Demonstration video link:

