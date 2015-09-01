function toauto(v, default)
	if(v == "true") then
		return true
	elseif(v == "false") then
		return false
	elseif(tonumber(v)) then
		return tonumber(v)
	elseif(v ~= "" and tostring(v)) then
		return tostring(v)
	end

	return default or nil
end