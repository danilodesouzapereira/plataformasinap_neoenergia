UPDATE (SELECT * FROM Rede) sel 
SET sel.intMRedeId = mrede_id 
WHERE sel.intEstudoId = [p_EstudoId] AND sel.intMRedeId = [p_RedeId];