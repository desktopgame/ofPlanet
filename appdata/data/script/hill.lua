topBlock = "GrassDirt"
fillBlockDeep = "Stone"
fillBlockShallow = "Dirt"
iBaseline = 0

function start()
    return "default"
-- return "ignore"
end

function onFixHeight(y)
    return y;
end

function onGenerateTerrain(x, y, z)
    startY = y
    setblock(x, y, z, topBlock);
    while y > 0 do
        y = y - 1
        if y < (startY - 5) then
            setblock(x, y, z, fillBlockDeep)
        else
            setblock(x, y, z, fillBlockShallow)
        end
    end
end

function onGenerateWater(x, y, z)
    setblock(x, y, z, "Water");
end

function onGenerateStructures()
end

function onGenerateCave(x, y, z, noise)
end

function onPostGenerate()
end