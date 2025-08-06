
// list of tiles to store search metadata (2d array the size of map)
// - visited flag to avoid reprocessing tiles, initialize to false
// - distance to reach this tile
// - best route to tile (the neighbor this tile was first reached from)

// priority queue (space for all tiles)
// - insert new candidate tile
// - select best candidate tile

// ensure player tile is not also enemy tile
// - game over
// - we probably check this before calling the pathfinding code
// initialize queue with just player tile
// while
// - select best candidate
// - for each neighbor
// - - if neighbor is enemy, return current tile (last step player->enemy == first step enemy->player)
// - - if not visited and not impassable
// - - - set visited flag
// - - - set distance
// - - - set route
// - - - insert to queue with priority: distance + estimated remaining distance
