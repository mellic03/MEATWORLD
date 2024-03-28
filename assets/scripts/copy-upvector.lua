function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    TransformSys.copySurfaceUp(subject, target);

    return 0;

end

