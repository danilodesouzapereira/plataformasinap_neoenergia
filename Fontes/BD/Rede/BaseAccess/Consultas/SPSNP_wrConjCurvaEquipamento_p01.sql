  UPDATE
    (SELECT * FROM Equipamento) sel
  SET
    sel.intConjCurvaId = [p_ConjCurvaId]
  WHERE
    sel.intEstudoId = [p_EstudoId] AND
    sel.intEquipId = [p_EquipId];
