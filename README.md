# Space collector

Space collector game

## Rules

- Square 20 000 x 20 000 kms
- Collect your planets with your collector
  - Slow speed
- Attack enemies with your five attackers
  - Fast speed
  - High energy attack < 5 000 kms
    - choose angle
    - 1 second to wait between fires of an attacker
- Explore with your explorer
  - Normal speed
  - See its planets and all spaceships around him < 5 000 kms
- When a unit is touched by a high energy attack
  - Must return to its base to be repaired
    - Attacker can't attack
    - Explorators can't use their radar to see enemy spaceships
    - Collectors can't collect planets, they loose the collected planets (left in place)
- When a team has collected all its planets, the game stops
- When the game ran 5 minutes, the game stops

## Commands

### General syntax

`COMMAND {ship_id} {parameters}`

- `{ship_id}`: identifier of the spaceship
  - 1, 2, 3, 4, 5: attackers
  - 6, 7: explorers
  - 8, 9: collectors
- `{parameters}`: parameters of the command
  - `{angle}`: integer, degrees, between 0 and 359, counter clockwise, 0 pointing right
  - `{speed}`: integer in kms/s

Each command returns a response, made with:

- `{planet_id}` is between 0 and 65535
- `{ship_id}` is between 1 and 9
- `{abscissa}` and `{ordinate}` are between 0 and 19 999

### Move

`MOVE {ship_id} {angle} {speed}`

Changes the speed and angle of the spaceship.

Maximum speed:

- 1 000 kms/s for collectors
- 2 000 kms/s for explorers
- 3 000 kms/s for attackers

Response is `OK`.

If a collector is less than 200 kms far from one of its planets, it collects the planet if it is not yet carrying a planet and it is not broken.

### Fire

`FIRE {ship_id} {angle}`

Fire a high energy attack, at `{angle}` angle. Length of the attack is 5 000 kms.

This command is only valid for an attacker.

Response is `OK`.

### Radar

`RADAR {ship_id}`

Starts the radar of an explorer.

Response is a one line string. It is composed of several elements, separated by commas. Those are based on entities at a distance less than 5 000 kms from the explorer. The elements are:

- `P {planet_id} {abscissa} {ordinate} {ship_id} {saved}`: one of your not yet collected planets, at a given position, the `ship_id` is the ID of the collector that collected the plane, or -1 if not collected, `saved` is 1 when planet is at base station, otherwise 0
- `S {team} {ship_id} {abscissa} {ordinate} {broken}`: a spaceship, team 0 is yours, team 1 to 3 are opponents, broken is 0 or 1, 1 meaning that the ship was targeted by a high energy attack (your spaceships are always present even if the explorer is broken)
- `B {abscissa} {ordinate}`: your base station's position (always present in radar information)

If an explorer is broken, it can't see enemy spaceships.
