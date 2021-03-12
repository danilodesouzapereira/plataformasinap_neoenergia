  UPDATE (SELECT * FROM Rede) sel
  SET sel.intAgrupamentoId = NULL 
  WHERE sel.intEstudoId = [p_EstudoId] AND sel.intAgrupamentoId = [p_EquipId];
