# Dungeon Crawler

A C++ project for the programming 2 class at HDA. 10*10 game field, with one human characters against multiple computer characters. The goal is to get to the final tile of the last level without dying. 

The game also features a Dijkstra algorithm visualizer as a fun add-on. Enable visualization mode on the dropbox after starting a game, and one of the characters (type Attacker) will explain to you how he implemented Dijkstra to find the shortest path to you!

# Built with
<img src="qt.webp" alt="drawing" width="50"/>

# Preview

<video width="320" height="240" controls>
  <source src="./preview.mov" type="video/mp4">
</video>

# Features

- Fighting : If a player enters a tile that is pre-occupied with an enemy (ie: human enters a tile that has a zombie, or vice versa, but not a zombie that enters a tile that has an attacker, because they are both computers), they must fight. Fighting decrements from both players' health points, if one dies the other can move to that tile.
- Dijkstra weapon : Double click on a tile, and its Dijkstra cost will be increased by 5, making it a less prefered tile for the Attacker. You are only to mark 3 tiles per level, use them wisely! 
- Save game state : Click on save at any point during the level,  and the level state will be saved, allowing you to resume the game later on without having to keep it on.

# Project Status

The game is currently functional and usable, but there is a lot of refactoring to do and hacky fixes to remove. The layouts especially aren't flexible at all. The visualization needs to be worked on to allow for interruption and rewinding (pause/next/previous buttons aren't functional), and it turns out that delaying dynamic memory deallocation till everything is finished wasn't a wise choice, will hopefully fix it in the semester break. 

# Reflection

This project was over a semester long, and being new to C++ and OOP generally meant that how I saw the big picture of the project changed every day I spent working on it. I've realized that initial design decisions shouldn't force future ones, and that refactoring was as important as writing new code. 

What I regret is not spending enough time learning Qt framework properly before starting, which made the design now "glued". I didn't care enough about it at the start because I thought its just the frontend, but if I did I wouldve saved much more time not having to keep trying different Qt methods until one works. 
