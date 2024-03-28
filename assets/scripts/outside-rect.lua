function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    if TransformSys.isInsideBoundingRect(subject, target) then
        return 0;
    end

    return 1;

end

