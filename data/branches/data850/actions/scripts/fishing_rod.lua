local FISH_WATER = {4608, 4609, 4610, 4611, 4612, 4613, 4614, 4615, 4616}
local NOFISH_WATER = {493, 4617, 4618, 4619, 4210, 4621, 4622, 4623, 4624, 4625, 4820, 4821, 4822, 4823, 4824, 4825}
local ICE_HOLE_FISH = 7236

local ITEM_NAIL = 8309
local ITEM_WORM = 3976
local ITEM_FISH = 2667
local ITEM_MECHANICAL_FISH = 10224

local ITEM_FISHING_ROD = 2580
local ITEM_MECHANICAL_FISHING_ROD = 10223
local ITEM_GREEN_PERCH = 7159
local ITEM_RAINBOW_TROUT = 7158
local ITEM_NORTHERN_PIKE = 2669

function onUse(cid, item, frompos, item2, topos)

	if (isInArray(NOFISH_WATER, item2.itemid) == TRUE) then
		doSendMagicEffect(topos, CONST_ME_LOSEENERGY)
		return TRUE
	end

	local canGainSkill = not(getTilePzInfo(getThingPos(cid)) == TRUE or
		(getPlayerItemCount(cid, ITEM_WORM) < 1 and item.itemid == ITEM_FISHING_ROD) or
		(getPlayerItemCount(cid, ITEM_NAIL) < 1 and item.itemid == ITEM_MECHANICAL_FISHING_ROD))

	local formula = (getPlayerSkill(cid, CONST_SKILL_FISHING) / 200) + (0.85 * math.random())
	local useNail = (item.itemid == ITEM_MECHANICAL_FISHING_ROD)

	-- First verify the most common case
	if (isInArray(FISH_WATER, item2.itemid) == TRUE) then
		-- The water has a fish. Verify if the player can gain skills
		if(canGainSkill) then
			if(formula > 0.7) then
				if useNail then
					doPlayerAddItem(cid, ITEM_MECHANICAL_FISH)
				else
					doPlayerAddItem(cid, ITEM_FISH)
				end
				doPlayerAddSkillTry(cid, CONST_SKILL_FISHING, 1)
				doTransformItem(item2.uid, item2.itemid + 9)
				doDecayItem(item2.uid)
			end
			doPlayerAddSkillTry(cid, CONST_SKILL_FISHING, 1)
		end
		doSendMagicEffect(topos, CONST_ME_LOSEENERGY)
	elseif (item2.itemid == ICE_HOLE_FISH) then
		if(canGainSkill) then
			if useNail then
				if formula < 0.5 then
					doPlayerAddItem(cid, ITEM_MECHANICAL_FISH)
				end
			else
				if(formula > 0.83) then
					doPlayerAddItem(cid, ITEM_RAINBOW_TROUT)
				elseif(formula > 0.75) then
					doPlayerAddItem(cid, ITEM_NORTHERN_PIKE)
				elseif(formula > 0.5) then
					doPlayerAddItem(cid, ITEM_GREEN_PERCH)
				else
					doPlayerAddItem(cid, ITEM_FISH)
				end
			end
			doTransformItem(item2.uid, item2.itemid + 1)
			doPlayerAddSkillTry(cid, CONST_SKILL_FISHING, 2)
		end
	else
		return FALSE
	end

	if useNail then
		doPlayerRemoveItem(cid, ITEM_NAIL, 1)
	else
		doPlayerRemoveItem(cid, ITEM_WORM, 1)
	end
	doDecayItem(item2.uid)

	return TRUE
end