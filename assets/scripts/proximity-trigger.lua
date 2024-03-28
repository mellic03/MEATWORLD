function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    if TransformSys.distance(subject, target) < 1.5 then
        print("[LUA]: progress-trigger-1.lua");
        return 1;
    end

    return 0;

end

