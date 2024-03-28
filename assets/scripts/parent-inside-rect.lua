function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    if TransformSys.isInsideBoundingRect(subject, target) then
        -- PlanetSys.disablePlanetActor(subject);
        -- ECS.giveChild(target, subject);
        return 1;
    else
        -- PlanetSys.enablePlanetActor(subject);
        -- ECS.removeParent(subject);
    end

    return 0;

end

