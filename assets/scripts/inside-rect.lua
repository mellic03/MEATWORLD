function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    if TransformSys.isInsideBoundingRect(subject, target) then
        return 1;
    end

    return 0;

end

