  DELETE FROM
    TrafoEnrol
  WHERE
    intEstudoId = [p_EstudoId] AND
    intTrafoId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN TrafoEnrol EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId  = EqpAux.intTrafoId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId IN (24, 25)
    );
