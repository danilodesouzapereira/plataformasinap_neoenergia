  DELETE RREquipamentoBarra.* FROM 
    RREquipamentoBarra INNER JOIN TmpEquipCorte ON
      RREquipamentoBarra.intMRedeId = TmpEquipCorte.intMRedeId AND
      RREquipamentoBarra.intEquipId = TmpEquipCorte.intEquipId AND
      RREquipamentoBarra.intCorteId = TmpEquipCorte.intCorteId
  WHERE
    intEstudoId = [p_EstudoId];
