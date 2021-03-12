  DELETE FROM
    Canal
  WHERE
    intEstudoId = [p_EstudoId] AND
    intCanalId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN Canal EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intCanalId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 32
    );