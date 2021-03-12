DELETE FROM Equipamento WHERE intEquipId IN (SELECT intEquipId FROM TmpEquipCurva) AND intEstudoId = [p_EstudoId];
