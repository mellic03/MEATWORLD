function ScriptEntry( subject, target )

    if (PhysXSys.hasCCT(subject) == false) then
        return 0;
    end

    if (TransformSys.distance(subject, target) > 1.5) then
        PhysXSys.moveTowardsCCT(subject, target, 0.1);
        return 0;
    end

    return 1;

end

