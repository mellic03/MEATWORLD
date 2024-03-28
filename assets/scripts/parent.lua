function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    ECS.giveChild(subject, target);

    return 0;

end

