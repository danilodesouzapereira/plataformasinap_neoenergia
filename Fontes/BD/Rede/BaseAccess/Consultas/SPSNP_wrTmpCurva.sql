    INSERT INTO
      TmpCurva (intConjCurvaId)
      SELECT 
        Equipamento.intConjCurvaId 
      FROM
        (
        ( 
        Equipamento INNER JOIN TmpEquip ON
           Equipamento.intEquipId = TmpEquip.intEquipId
        )
        INNER JOIN ConjuntoCurva ON
           Equipamento.intConjCurvaId = ConjuntoCurva.intConjCurvaId
        )
      WHERE
        Equipamento.intEstudoId = [p_EstudoId] AND
        Equipamento.bolAlteradoObra = 1 AND
        ConjuntoCurva.bolTipica = 0
