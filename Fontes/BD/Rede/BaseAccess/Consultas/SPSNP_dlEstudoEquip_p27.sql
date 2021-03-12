  DELETE FROM
    Trafo3E
  WHERE
    intEstudoId = [p_EstudoId] AND
    intTrafo3EId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN Trafo3E EqpAux ON
        Equipamento.intEstudoId  = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intTrafo3EId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 25
    );
