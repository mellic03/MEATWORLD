function ScriptEntry( subject, target )

    local d = TransformSys.distance(subject, target);

    if (d < 0.5) then
        ECS.loadScene("scene-C.idksc");
    end

end