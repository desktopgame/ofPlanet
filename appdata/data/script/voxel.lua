posX = 64
posY = 32
posZ = 64

function start()
    newstruct("Model",[[
,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
]]) 
    return "default"
-- return "ignore"
end

function onFixHeight(y)
    return y
end

function onGenerateTerrain(x, y, z)
end

function onGenerateWater(x, y, z)
end

function onGenerateStructures()
    expandstruct(posX, posY, posZ, "Model")
end

function onGenerateCave(x, y, z, noise)
end

function onPostGenerate()
end