function ScriptEntry( subject, target )

    local d = TransformSys.distance(subject, target);

    if (d < 1.0) then
        ECS.loadScene("scene-B.idksc");
    end

end