function ScriptEntry( subject, target )

    if subject == nil or target == nil then
        return 0;
    end

    GameStateSys.incrementProgress(subject);

    return 0;

end

