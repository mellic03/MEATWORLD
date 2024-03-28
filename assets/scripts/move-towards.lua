function ScriptEntry( subject, target )

    if (TransformSys.distance(subject, target) > 1.5) then
        TransformSys.moveTowards(subject, target, 4.0*Engine.deltaTime());
        return 0;
    end

    return 1;

end

