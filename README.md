# GenericNPCAI

An unreal project to create and experiment with a utility AI based system for use by NPCs in Games.

[video demo 1](https://youtu.be/T-m9Xv3hVhY)
 
[video demo 2](https://youtu.be/t9uvMAhkY3o)


## General Approach

The system consists of two steps for each NPC.

Step 1:
we check the game world state info for the NPC and list all the things the NPC ***COULD DO***. These are called ***GOALS***. Each goal has an associated ***BEHAVIOUR*** which is basically a sequence of actions the NPC must take to Achieve the goal

Step 2:
We now need to select which of the goals from step 1 the NPC should act on. ie, We need to decide what the NPC ***SHOULD DO***. For this we compute a score for all the behaviours associated with the goals from step 1 and execute the behaviour 
with the highest score. Each behaviour also has some preconditions which are required ( mostly the associated goal being available) for its score to be considered

we just repeat steps 1 and 2 every few frames in the game to generate new goals and select new behaviours to run based on the game state.
 
 
 
## Pros and Cons over other NPC AI implementations like Behaviour trees

### Pros
This sytem is much more modular and reusable compared to traditional behaviour trees as its easier to compose the AI of different NPCs by just adding the necessary goal generators and behaviours to them. It is also possible to add/remove behaviors and
goals in runtime as needed. (for example lets say a NPC gets a mind controlled status. you can just add in some goal generators and behaviours for handling this and then remove them when the NPC gets the mind control removed all in run time)

Utility AI also leads to more emergent unscripted behaviours from NPCs as compared to rigid behaviour trees

### Cons
Utility AI is harder to debug when the NPC does something unwanted. The scores are also harder to work with for AI designers to implement specific controlled behaviours. In general other AI methods like behaviour trees are preferable in cases where
we want our NPCs to have strictly controlled scripted behaviours



## Components of the AI System
The main components are the GoalGenerators, GoalDatas, Behaviours and the BehaviourSelector.

### Goal Generator
Goal generators create modify and destroy goals based on the current gamestate. A single goal generator can manage several different goals for an NPC. for example a Combat Goal Generator can manage combat, flank and chase goals.
These goals are all represented in memory as Goal Datas. All Goal Generators inherit from the UBaseGoalGenerator and can be implemented either in C++ or blueprints

### Goal Data
This is a data container which contains all the necessary info an Behaviour associated with the goal needs to run. All Goal datas are inherited from UBaseGoalData and can be defined in C++ or blueprints

### Behaviour Selector
The behaviour Selector on each NPC has a list of Behaviour classes which are instantiated in runtime. Based on the behaviours' preconditions and utility score it selects a behaviour to run. The behaviour selector continues to tick the 
running behaviour until it signals that its complete and then runs the selection process again. There is also a mechanism to interrupt a running behaviour if its in an interruptible state and a higher scored behaviour is available

### Behaviour
A class inheriting from UBaseBehaviour which defines the sequence of actions to take to achieve a goal. it also defines a way to evaluate its selection score and the preconditions required for it to run. Behaviours can be implemented in C++ or blueprints




## How to use this project for your own experiments
You can download the open the project using Unreal Engine. The unreal version used for this project is 5.1.1, some of the demo assets may not be compatible wiṭh other versions.

The project also contains a scenario with two teams of AI shooters fighting each other to demonstrate the system. The maps for this demo is the Playground map in [/Content/MyContent/Maps/](/Content/MyContent/Maps/)

### How to create your own goal generators and behaviours

You can create your own Goal Generators, Goal Datas and Behaviours by inheriting from UBaseGoalGenerator, UBaseGoalData and UBaseBehaviour classes respectively in either blueprints or C++ respectively (if you do implement in c++ I still recommend
creating a BP of the class just to set its class default values. for example a behaviour needs to set its Goal Data Class field to associate it with a goaldata.) and then overriding the methods in them.

You can find some examples under [/Content/MyContent/UtilityAI](/Content/MyContent/UtilityAI)

In order to use this system on a pawn/character you need to add the following to it:
  *BehaviourSelectorComponent
  *GoalGeneratorComponent
  *Set the AI controller of your pawn to the UtilityAIAIController in [/Content/MyConten/UtilityAI](/Content/MyContent/UtilityAI) 

Then you just need to populate the Goal generator and Behaviour lists in the properties of GoalGeneratorComponent and BehaviourSelectorComponent respectively.




