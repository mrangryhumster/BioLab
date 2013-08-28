-- @autor: AngryHumster(aka creator BLab)

function move(x,y)
	if action("move",x,y) == false
	then
		ID,Health,Team = inspect(x,y)
		sID,sx,sy,sHealth,sActions,sTeam = getFullStatus()
		if ID ~= nil
		then
			if Team ~= sTeam
			then
				action("attack",x,y)		
			end
		end
	end
end

function kill(x,y)
		ID,Health,Team = inspect(x,y)
		sID,sx,sy,sHealth,sActions,sTeam = getFullStatus()
		if ID ~= nil
		then
			if Team ~= sTeam
			then
				action("attack",x,y)		
			end
		end
end

function catch(x,y)
		ID,Health,Team = inspect(x,y)
		sID,sx,sy,sHealth,sActions,sTeam = getFullStatus()
		if ID ~= nil
		then
			if Team ~= sTeam
			then
				action("move",x,y)		
			end
		end
end

function runaway(x,y)
		ID,Health,Team = inspect(x,y)
		sID,sx,sy,sHealth,sActions,sTeam = getFullStatus()
		if ID ~= nil
		then
			if Team ~= sTeam
			then
				action("move",-x,-y)		
			end
		end
end


function kill_someone()
kill(-1,-1)
kill( 0,-1)
kill( 1,-1)
kill( 1, 0)
kill( 1, 1)
kill( 0, 1)
kill(-1, 1)
kill(-1, 0)
end

-- *****************************************************************************
sID,sx,sy,sHealth,sActions,sTeam = getFullStatus()


for xp = -2,2,1
do

for yp = -2,2,1
do
if sHealth >= 50
then
	catch(xp,yp)
else
	runaway(xp,yp)
end
	end

end

if sHealth <= 50
then 
rest()
else
kill_someone()
end

rnd = getRandom(4)

if rnd == 0
then
move(1,0)
end

if rnd == 1
then
move(-1,0)
end

if rnd == 2
then
move(0,1)
end

if rnd == 3
then
move(0,-1)
end

-- ******************************************************************************
