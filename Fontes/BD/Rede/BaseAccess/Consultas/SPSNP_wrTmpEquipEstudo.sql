    INSERT INTO
      TmpEquip (intEquipId)
      SELECT intEquipId FROM Equipamento WHERE intEstudoId = [ p_EstudoId ];
