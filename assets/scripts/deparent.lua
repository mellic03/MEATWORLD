function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    ECS.removeParent(subject);

    return 0;

end

