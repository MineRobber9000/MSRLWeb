import "Sprite"

//----------------------------------------------------------------------

ship = new Sprite
ship.init = function
	self.srcTexture = spriteSheetTex
	self.srcRectEngineOff = [32*6, 32*8, 32*3, 32*2]
	self.srcRectEngineOn =  [32*9, 32*8, 32*3, 32*2]
	self.srcRect = self.srcRectEngineOff
	self.x = 480
	self.y = 320
	self.scale = 0.5
	self.rotSpeed = 10
end function

ship.update = function
	if raylib.IsKeyDown(raylib.KEY_LEFT) then self.rotation -= self.rotSpeed
	if raylib.IsKeyDown(raylib.KEY_RIGHT) then self.rotation += self.rotSpeed
end function

//----------------------------------------------------------------------

init = function
	outer.spriteSheetTex = raylib.LoadTexture("assets/Asteroids-2X.png")
	ship.init
end function

update = function
	ship.update
end function

render = function
	raylib.BeginDrawing
	raylib.ClearBackground raylib.BLACK

	//raylib.DrawTexture spriteSheetTex //, x, y, raylib.WHITE
	ship.draw
	
	raylib.DrawFPS
	raylib.EndDrawing
end function

run = function
	init
	while true
		update
		render
		yield
	end while	
end function

if locals == globals then run
