function ScriptEntry( subject, target )

    local d = TransformSys.distance(subject, target);

    if (d < 1.0) then
        ECS.loadScene("scene-A.idksc");
    end

end