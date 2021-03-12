  DELETE FROM
    RREquipamentoBarra
  WHERE
    intEstudoId = [p_EstudoId] AND
    intMRedeId = [p_MRedeId] AND
    intEquipId = [p_EquipId] AND
    intCorteId = [p_CorteId];
