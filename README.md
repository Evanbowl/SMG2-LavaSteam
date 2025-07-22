`LavaSteam` (Lava Steam) is an actor found in a few *Super Mario Galaxy 1* galaxies, most notably Melty Molten Galaxy.<br/>
The hot steam it emits damages the player on contact.<br/>
This actor is not present in *Super Mario Galaxy 2*.<br/><br/>

Lava Steam has three distinct phases.<br/>
Phase 1: The Lava Steam is inactive.<br/>
Phase 2: Sign: The Lava Steam emits a small and harmless steam cloud.<br/>
Phase 3: Steam: The Lava Steam emits a large steam cloud and damages the player on contact.<br/>

The `SW_SLEEP` functionality has been removed from Lava Steam since this Switch option as a whole is not present in SMG2.<br/>


This variant of `LavaSteam` comes with new parameters.

|Parameter|Data Type|Function|Description|Default Values|
|-|-|-|-|-|
|`Obj_arg0`|long|Cycle Interval|Time interval, in frames, between cycles.|90 frames|
|`Obj_arg1`|long|Sign Time|Time, in frames, of the Sign phase.|90 frames|
|`Obj_arg2`|long|Steam Time|Time, in frames, of the Steam phase.|90 frames|
|`Obj_arg3`|bool|Sign on Activation?|On `SW_A` or `Demo` activation, start from the Sign phase instead of Steam phase on activation.|false (Steam Phase)|
|`Obj_arg4`|bool|Steam Forever?|If activated, the Steam phase never stops.|false|
|`SW_A (Read)`|-|Activate|Activates and starts from the phase defined in `Obj_arg3`.|-|
|`SW_B (Read)`|-|Deactivate|Deactivates the steam emission.|-|
|Demo|-|Activate|Activates and starts from the phase defined in `Obj_arg3`.<br/>Use `ActionType` 3 in the demo sheet.|-|<br/>

To use Lava Steam in a demo, the following entry must be included in the `ObjNameTable`.<br/>
You will need to refer to `jp_name` when referencing the Lava Steam in a demo.<br/>

|en_name|jp_name|
|-|-|
|`LavaSteam`|`溶岩スチーム`|