# GenericNPCAI

An unreal project to create and experiment with a utility AI based system for use by NPCs in Games. The project also contains scenario with two teams of AI shooters fighting each other to demonstrate the system.


## General Approach
The system consists of two steps for each NPC.

Step 1:
we check the game world state info for the NPC and list all the things the NPC **COULD DO**. These are called **GOALS**. Each goal has an associated **BEHAVIOUR** which is basically a sequence of actions the NPC must take to Achieve the goal

Step 2:
We now need to select which of the goals from step 1 the NPC should act on. ie, We need to decide what the NPC **SHOULD DO**. For this we compute a score for all the behaviours associated with the goals from step 1 and execute the behaviour 
with the highest score.

we just repeat steps 1 and 2 every few frames in the game to generate new goals and select new behaviours to run based on the game state.


## Components of the AI System
The main components are the GoalGenerators, GoalDatas, Behaviours and the BehaviourSelector.




